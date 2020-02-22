#include "CgiConnect.h"

int cgiConnect :: sockFd  ;
struct sockaddr_in cgiConnect:: addr  ;
char cgiConnect :: buf[BUFFERSIZE] = {'0'};

cgiConnect :: cgiConnect() {
}   

int cgiConnect ::connectCgiServer() {
    auto  err = log::getLogObject() ;
    sockFd = socket(AF_INET, SOCK_STREAM, 0) ;
    if(sockFd < 0) {
        std::string s =  std::to_string(__LINE__)+"       " + __FILE__ ;
        (*err)<<s ;
        return -1 ;
    }
    auto res = configure::getConfigure() ;
    addr.sin_family = AF_INET ;
    addr.sin_port = htons(res->getCgiPort()) ;
    addr.sin_addr.s_addr = inet_addr(res->getCgiIP().c_str()) ;
    int ret = connect(sockFd, (struct sockaddr*)&addr, sizeof(addr)) ;
    if(ret < 0) {
        std::string s =  std::to_string(__LINE__)+"       " + __FILE__ ;
        (*err)<<s ;
        return -1 ;
    }
    return sockFd ;
}

int cgiConnect :: sendMsg(const char*  buffer) {
    auto  err = log::getLogObject() ;
    strcpy(buf, buffer) ;
    if(send(sockFd, buf, sizeof(buf), 0) < 0) {
        std::string s =  std::to_string(__LINE__)+"       " + __FILE__ ;
        (*err)<<s ;
        return -1 ;
    }
    return 1 ;
}

std::string cgiConnect :: recvMsg() {
    auto  err = log::getLogObject() ;
    std::string ss ;
    if(recv(sockFd, buf, sizeof(buf), 0)<0) {       
        std::string s =  std::to_string(__LINE__)+"       " + __FILE__ ;
        (*err)<<s ;
        return "" ;
    } 
    ss += buf ;
    return ss ;
}
