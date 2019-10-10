#pragma once
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define IP  "127.0.0.1" 
#define PORT 7000 
const int BUFFERSIZE = 1024 ;
using namespace std ;

class cgiConnect {
public:
    cgiConnect() ;
    ~cgiConnect() {close(sockFd) ;}
    static int connectCgiServer() ;
    static string recvMsg() ;
    static int sendMsg(const char* buf) ;
    static void closeFd() {close(sockFd) ;}
private:
    static int sockFd ;
    static struct sockaddr_in addr ;
    static char buf[BUFFERSIZE] ;
};

