#include "FastCgi.h"
#include "Fcgi.h"

#include <string>
#include<iostream>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

FastCgi::FastCgi()
{
    sockfd_ = 0;
    requestId_ = 0;
    flag_ = 0;
}


FastCgi::~FastCgi()
{
    close(sockfd_);
}

//此处需要修改
int FastCgi:: sendRequest(const char* content, long len) {
    return send(sockfd_, content, len, 0) ;    
}

int FastCgi:: sendRequest(const FCGI_Header head) {
    return send(sockfd_, &head, sizeof(head), 0) ;
}

FCGI_Header FastCgi::makeHeader(int type,int requestId,int contentLength,int paddingLength)
{
    FCGI_Header header;
    
    header.version = FCGI_VERSION_1;
    
    header.type    = (unsigned char)type;   
    
    header.requestIdB1 = (unsigned char)((requestId >> 8) & 0xff);  //用连个字段保存请求ID
    header.requestIdB0 = (unsigned char)(requestId & 0xff);
  
    header.contentLengthB1 = (unsigned char)((contentLength >> 8) & 0xff);//用俩个字段保存内容长度
    header.contentLengthB0 = (unsigned char)(contentLength & 0xff);

    header.paddingLength = (unsigned char)paddingLength;        //填充字节的长度

    header.reserved = 0;    //保留字节赋为0

    return header;
}


FCGI_BeginRequestBody FastCgi::makeBeginRequestBody(int role,int keepConnection)
{
    FCGI_BeginRequestBody body;

    body.roleB1 = (unsigned char)((role >> 8) & 0xff);//俩个字节保存我们期望php-fpm扮演的角色
    body.roleB0 = (unsigned char)(role & 0xff);

    body.flags = (unsigned char)((keepConnection) ? FCGI_KEEP_CONN : 0);//大于0常连接，否则短连接

    bzero(&body.reserved,sizeof(body.reserved));

    return body;
}


bool FastCgi::makeNameValueBody(std::string name,int nameLen,std::string value,int valueLen,
                                unsigned char *bodyBuffPtr,int *bodyLenPtr)
{
    unsigned char *startBodyBuffPtr = bodyBuffPtr;  //记录body的开始位置
    
    if(nameLen < 128)//如果nameLen长度小于128字节
    {
        *bodyBuffPtr++ = (unsigned char)nameLen;    //nameLen用一个字节保存
    }
    else
    {
        //nameLen用4个字节保存
        *bodyBuffPtr++ = (unsigned char)((nameLen >> 24) | 0x80);
        *bodyBuffPtr++ = (unsigned char)(nameLen >> 16);
        *bodyBuffPtr++ = (unsigned char)(nameLen >> 8);
        *bodyBuffPtr++ = (unsigned char)nameLen;
    }

    if(valueLen < 128)  //valueLen小于128就用一个字节保存
    {
        *bodyBuffPtr++ = (unsigned char)valueLen;
    }
    else
    {
        //valueLen用4个字节保存
        
        *bodyBuffPtr++ = (unsigned char)((valueLen >> 24) | 0x80);
        *bodyBuffPtr++ = (unsigned char)(valueLen >> 16);
        *bodyBuffPtr++ = (unsigned char)(valueLen >> 8);
        *bodyBuffPtr++ = (unsigned char)valueLen;
    }

    //将name中的字节逐一加入body的buffer中
    for(auto ch : name)
    {
        *bodyBuffPtr++ = ch;
    }

    //将value中的值逐一加入body的buffer中
    for(auto ch : value)
    {
        *bodyBuffPtr++ = ch;
    }

    //计算出body的长度
    *bodyLenPtr = bodyBuffPtr - startBodyBuffPtr;
    return true;
}


void FastCgi::startConnect(void)
{
    int sockfd;
    struct sockaddr_in server_address;

    //获取配置文件中的ip地址
    //std::string ip = getIpFromConf();
    std:: string ip = "127.0.0.1" ;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd > 0);

    bzero(&server_address,sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip.c_str());
    server_address.sin_port = htons(9000);
    int result = connect(sockfd,(struct sockaddr *)&server_address,sizeof(server_address));
    if(result < 0) {
        std:: cout << __FILE__ << "      " << __LINE__ << std::endl ;
    }
    assert(result >= 0);
    sockfd_ = sockfd;
}


bool FastCgi::sendStartRequestRecord(void)
{
    FCGI_BeginRequestRecord beginRecord;

    beginRecord.header = makeHeader(FCGI_BEGIN_REQUEST,requestId_,sizeof(beginRecord.body),0);
    beginRecord.body = makeBeginRequestBody(FCGI_RESPONDER,0);

    int ret = write(sockfd_,(char *)&beginRecord,sizeof(beginRecord));
    assert(ret == sizeof(beginRecord));
    return true;
}


bool FastCgi::sendParams(std::string name,std::string value)
{
    unsigned char bodyBuff[PARAMS_BUFF_LEN];
    
    bzero(bodyBuff,sizeof(bodyBuff));
    
    int bodyLen;//保存body的长度
    
    //生成PARAMS参数内容的body
    makeNameValueBody(name,name.size(),value,value.size(),bodyBuff,&bodyLen);
    
    FCGI_Header nameValueHeader;
    nameValueHeader = makeHeader(FCGI_PARAMS,requestId_,bodyLen,0);

    int nameValueRecordLen = bodyLen + FCGI_HEADER_LEN;
    char nameValueRecord[nameValueRecordLen];

    //将头和body拷贝到一块buffer中只需调用一次write
    memcpy(nameValueRecord,(char *)&nameValueHeader,FCGI_HEADER_LEN);
    memcpy(nameValueRecord + FCGI_HEADER_LEN,bodyBuff,bodyLen);

    int ret = write(sockfd_,nameValueRecord,nameValueRecordLen);
    assert(ret == nameValueRecordLen);

    return true;
}


bool FastCgi::sendEndRequestRecord(void)
{
    FCGI_Header endHeader;

    endHeader = makeHeader(FCGI_PARAMS,requestId_,0,0);

    int ret = write(sockfd_,(char *)&endHeader,FCGI_HEADER_LEN);
    if(ret < 0) {
        return -1 ;
    }
    assert(ret == FCGI_HEADER_LEN);

    return true;
}


std::string FastCgi::readFromPhp(void)
{
    FCGI_Header responderHeader;
    char content[CONTENT_BUFF_LEN];
    strcpy(content, "") ;
    int contentLen;
    char tmp[8];    //用来暂存padding字节的
    int ret;
    //先将头部8个字节度出来
    while(read(sockfd_,&responderHeader,FCGI_HEADER_LEN) > 0)
    {
        if(responderHeader.type == FCGI_STDOUT)
        {
            //获取内容长度
            contentLen = (responderHeader.contentLengthB1 << 8) + (responderHeader.contentLengthB0);
            printf("内容的长度：%d\n", contentLen) ;
            bzero(content,CONTENT_BUFF_LEN);
            //读取获取的内容
            ret = read(sockfd_,content,contentLen);
            assert(ret == contentLen);
            //printf("ret = %d\n",ret);
            getHtmlFromContent(content);
            
            //跳过填充部分
            if(responderHeader.paddingLength > 0)
            {
                ret = read(sockfd_,tmp,responderHeader.paddingLength);
                assert(ret == responderHeader.paddingLength);
            }
        }
        else if(responderHeader.type == FCGI_STDERR)
        {
            contentLen = (responderHeader.contentLengthB1 << 8) + (responderHeader.contentLengthB0);
            bzero(content,CONTENT_BUFF_LEN);
            ret = read(sockfd_,content,contentLen);
            assert(ret == contentLen);

            fprintf(stdout,"error:%s\n",content);

            //跳过填充部分

            if(responderHeader.paddingLength > 0)
            {
                ret = read(sockfd_,tmp,responderHeader.paddingLength);
                assert(ret == responderHeader.paddingLength);
            }

        }
        else if(responderHeader.type == FCGI_END_REQUEST)
        {
            FCGI_EndRequestBody endRequest;

            ret = read(sockfd_,&endRequest,sizeof(endRequest));
            assert(ret == sizeof(endRequest));

            //fprintf(stdout,"endRequest:appStatus:%d\tprotocolStatus:%d\n",(endRequest.appStatusB3 << 24) + (endRequest.appStatusB2 << 16)
              //      + (endRequest.appStatusB1 << 8) + (endRequest.appStatusB0),endRequest.protocolStatus);
        }
    }
    return content;
}


char *FastCgi::findStartHtml(char *content)
{
    for(;*content != '\0';content++)
    {
        
        if(*content == '<')
            return content;
    }

    return NULL;
}

void FastCgi::getHtmlFromContent(char *content)
{
    char *pt;   //保存html内容开始位置

    if(1 == flag_)   //读取到的content是html内容
    {
        printf("%s",content);   
    }
    else
    {
        if((pt = findStartHtml(content)) != NULL)
        {
            flag_ = 1;
            for(char* i = pt;*i != '\0';i++)
            {
                printf("%c",*i);
            }        
        }          
    }    
}
