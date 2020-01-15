#ifndef _SOCKET_H_
#define _SOCKET_H_
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include <string>
#include<netinet/in.h>
#include<fcntl.h>
#include<stdlib.h>
#include <sys/un.h>
#include<string.h>
#include<assert.h>
#define BACKLOG 1024
#define PATH "/tmp/sss"
using namespace std ;
//创建Unix套接字,连接服务器
class unixSocket {
public :
    unixSocket() {
        un.sun_family = AF_UNIX ;
        strcpy(un.sun_path, PATH) ;
        len = sizeof(un.sun_family)+strlen(un.sun_path) ;
        sockFd = socket(AF_UNIX, SOCK_STREAM, 0) ;
        if(connect(sockFd, (sockaddr*)&un, len) < 0) {
            std::cout << __FILE__ << "    " <<__LINE__ << std::endl ;
            return ;
        } 
    }
    ~unixSocket() {close(sockFd) ;}
public :
    int getFd() { return sockFd ; }
public :
    int len ;
    int sockFd ;
    sockaddr_un un ;
} ;

//创建网络套接字
class socketFd
{
public:
    socketFd() ;
    socketFd(const char* port) ;
    socketFd(const std::string addr, const std::string port);
    socketFd(int port) ;
    ~socketFd() {
        close(connFd) ;
        close(sockFd) ;
    } ;

public :
    void setListenFd(int fd) { sockFd = fd ; }
    void setConnFd(int fd) { connFd = fd ; }
    int getListenFd() { return sockFd ;}
    //设置地址
    int setAddr(int port) ;
    int setAddr(const char* ip, const char* port) ;
    int setAddr(const char* port) ;
    //套接字绑定地址
    int bindAddress() ;
    //设置客户端延迟关闭选项
    int setLinger(int fd) ;
    //开始监听
    int startListen() ;
    //地址复用
    int setReuseAddr() ;
    int setReusePort() ;
    //接收新连接
    int acceptSocket() ;
    //获取监听套接字
    int getListenSock() ;
    //获取接收到的新连接套接字
    int getAcceptSock() ;
    //社会套接字非阻塞
    int setNoBlocking(int fd) ;
    //关闭读端
    void shutdownWrite(int fd) ;
    //是否绑定过地址
    void setBindAddr(int i) { isBindAddr = i; }
    int getBindAddr() { return isBindAddr ; }
    string getIp() { return ip ; }
    string getPort() { return port ; }
private:
    int isBindAddr = 0 ;
    int sockFd = -1 ;
    int connFd =  -1 ;
    struct sockaddr_in sockAddr ;
    string ip ;
    string port ;
};

#endif
