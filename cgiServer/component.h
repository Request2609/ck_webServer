#pragma once
#include <iostream>
#include <assert.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <memory>
#include <sys/types.h>
#include <unistd.h>
#include <map>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <vector>
#include <memory> 
#include "Log.h"

const int BUFFERSIZE = 1024 ;
class tool {
public :
    static int setNoBlock(int fd) ;
    static int addFd(int epollFd, int fd) ;
    static int removeFd(int epollFd, int fd) ;
    static void addSig(int sig, void(handle)(int), bool restart=true) ;
    static int createSocketPair(int* pipe) ;
    static int createEventFd() ;
    static std::shared_ptr<log>err ;
    static int sigOK ;
};

class process {
public:
    process() : pid(-1){}
    ~process() {}
public :
    int createSocketPair() ;
    int getReadFd() { 
        return pipe[0] ; 
    }
    int getWriteFd() { 
        return pipe[1] ; 
    }
public :
    pid_t pid ;
private :
    int pipe[2] ;
};

struct cgiData {
    int isEof ;
    char path[FILENAME_MAX] ;
    char body[BUFFERSIZE] ;
    char contentType[BUFFERSIZE] ;
    char requestType[BUFFERSIZE] ;
} ;

class cgiConn {
public :
    cgiConn(int epollFd, int fd) {
        cgiConn::epollFd = epollFd ;
        sockFd = fd ;
        memset(buf, '\0', BUFFERSIZE) ;
        endIndex = 0 ;
    }
    ~cgiConn() {
        close(sockFd) ;
    }
    void process() ;
    void getArg(int& a, int& b) ;
    int cgiProcess(int sockFd, std::string path, std::string arg) ;
    //获取参数
public :
    struct cgiData cd ;
    //cgi路径
    //cgi参数
    std::string cgiArg ;
    static int epollFd ;
    int sockFd ;
    char buf[BUFFERSIZE] ;
    int endIndex ;
} ;


