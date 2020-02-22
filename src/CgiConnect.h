#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include "Log.h"
#include "ParseConf.h"

const int BUFFERSIZE = 1024 ;

struct cgiData {
    bool isEOF ;
    char path[FILENAME_MAX] ;
    char body[BUFFERSIZE] ;
    char contentType[BUFFERSIZE] ;
    char requestType[BUFFERSIZE] ;
} ;


class cgiConnect {
public:
    cgiConnect() ;
    ~cgiConnect() {
        close(sockFd) ;
    }
    static int connectCgiServer() ;
    static std::string recvMsg() ;
    static int sendMsg(struct cgiData& buf) ;
    static void closeFd() {
        close(sockFd) ;
    }
private:
    static int sockFd ;
    static struct sockaddr_in addr ;
    static char buf[BUFFERSIZE] ;
};

