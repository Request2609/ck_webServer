#ifndef _PROCESS_H_
#define _PROCESS_H_
#include <unistd.h>
#include<iostream>
#include<string>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include<sys/stat.h>
#include "CgiConnect.h"
#include "Fcgi.h"
#include "Channel.h"
#include "ReadWrite.h"
#include "Socket.h"
#include "SendFile.h"
#include "EventLoop.h"
#include "ObjectPool.h"
#include "ParseConf.h"
#include "Log.h"


enum {
    GET = 1, POST, DEFAULT
} ;

enum {
    NOT_FOUND=404, OK=200
} ;

class eventLoop ;
class process {
public:
    process():paths(""), method(-1), version(""), flag(0) {}
    ~process() {}
public :
    void fastCgi() ;
    int getRequest(channel* chl, std::string& tmp) ;
    int postRequest(std::string& tmp, 
                    channel* chl, std::string& bf) ;
    int isExist() ;
    int requestHeader(channel* channel_, 
                      std::vector<std::pair<int, 
                      std::shared_ptr<channel>>>& mp) ;
    int requestBody(channel* channel_) ;
    int getMethod(std::string& line) ;
    int messageSend(const std::string& tmp, channel*chl) ;
    int getVersionPath(std::string  tmp) ;
    void responseHead(channel* chl, std::string type, 
                      long len, int statusCode, std::string tip) ;
    void readBigFile(channel* chl, int fd, unsigned long len) ;
    void sendNotFind(channel* chl) ;
    std::string getFileType() ;
    int processArgGet(std::string tmp, channel* chl) ;
    int getContentLength(std::string a, channel* chl) ;  
    int getSubmitInfo(std::string& info, int pos, 
                      int l, std::string& a, channel* chl) ;
    int doPost(std::string& info) ;
    std::string changeHtml() ;
    std::string changePostHtml(long len, std::string&bf) ;
    std::string getSubmit(long len, std::string&bf) ;
    int sendCgiResult(channel * chl, std::string res) ;
    std::string processCgi() ;
    void getSendBuffer(channel* chl, const std::string res) ;
    void sendBuffer(channel* chl) ;
    int sendHeader(channel* chl) ;
    int sendfiles(channel* chl, int fd, int size) ;
    void readFile(std::string filename) ;
    void chunkEncodingHead() ;
    int getFileInfo(int& fd, long& len, const char* paths) ;
    int sendLittleFile(channel* chl, long len, int fd) ;
    bool isConnect(const std::string& a) ;
    void processDisConnect( channel* chl,  
                            std::vector<std::pair<int, 
                            std::shared_ptr<channel>>>& mp) ;
private :
    std::string post ;
    std::string paths ;
    int method ;
    std::string version ;
    //是get请求的话，就设置为1
    int flag ;
    std::string cgiArg ;
    sendFile sss ;
    int canDel ;
    std::shared_ptr<log>err ;
    std::string connection ;
};
#endif
