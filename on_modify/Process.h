#ifndef _PROCESS_H_
#define _PROCESS_H_
#include <unistd.h>
#include<iostream>
#include<string>
#include<sys/stat.h>
#include "CgiConnect.h"
#include "Fcgi.h"
#include"Channel.h"
#include"ReadWrite.h"
#include "Socket.h"
#include "SendFile.h"
#define DEFAULT_PATH "index.html"
#define BUFLEN 65535 

#define FASTCGI 1
#define CGI 2 

using namespace std ;
enum {
    GET = 1, POST, DEFAULT
} ;

enum {
    NOT_FOUND=404, OK=200
} ;
//向进程发送的登录信息
struct logBuf {
    //暂时业务为登录
    int flag ;
    int type ;
    char name[512] ;
    char password[512] ;
    char version[1024] ;
    char path_[1024] ;
} ;

class process
{
public:
    process():paths(""), method(-1), version(""), flag(0) {}
    ~process() {}
public :
    void fastCgi() ;
    int getRequest(channel* chl, string& tmp) ;
    int postRequest(string& tmp, channel* chl, string& bf) ;
    int isExist() ;
    int requestHeader(channel* channel_, vector<pair<int, shared_ptr<channel>>>& mp) ;
    int requestBody(channel* channel_) ;
    int getMethod(string& line) ;
    int messageSend(const string& tmp, channel*chl) ;
    int getVersionPath(string  tmp) ;
    void responseHead(channel* chl, string type, long len, int statusCode, string tip) ;
    void readFile(const char* file, channel* chl) ;
    void sendNotFind(channel* chl) ;
    void readFile(channel* chl) ;
    string getFileType() ;
    int processArgGet(string tmp, channel* chl) ;
    int getContentLength(string a, channel* chl) ;  
    int getSubmitInfo(string& info, int pos, int l, string& a, channel* chl) ;
    int doPost(string& info) ;
    int sendSock(logBuf& buf, int fd, int connFd) ;
    string changeHtml() ;
    string changePostHtml(long len, string&bf) ;
    string getSubmit(long len, string&bf) ;
    int sendCgiResult(channel * chl, string res) ;
    string processCgi() ;
    void getSendBuffer(channel* chl, const string res) ;
    void sendBuffer(channel* chl) ;
private :
    string post ;
    string paths ;
    int method ;
    string version ;
    //是get请求的话，就设置为1
    int flag ;
    string cgiArg ;
    sendFile sss ;
    int canDel ;
};
#endif
