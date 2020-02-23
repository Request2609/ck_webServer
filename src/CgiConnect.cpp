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

int cgiConnect :: sendMsg(struct cgiData& cd) {
    auto  err = log::getLogObject() ;
    if(send(sockFd, &cd, sizeof(cd), 0) < 0) {
        std::string s =  std::to_string(__LINE__)+"       " + __FILE__ ;
        (*err)<<s ;
        return -1 ;
    }
    return 1 ;
}

std::string cgiConnect :: recvMsg() {
    int len = 0 ;
    int index= 0 ;
    int size ;
    auto  err = log::getLogObject() ;
    std::string ss="" ;
    std::string buffer ;
    char buf[BUFSIZ] ;
    while(1) {
        if(recv(sockFd, buf, sizeof(buf), 0)<0) {       
            std::string s =  std::to_string(__LINE__)+"       " + __FILE__ ;
            (*err)<<s ;
            return "" ;
        }
        ss = buf ;
        if(ss.find("\r\n") == std::string::npos) {
            continue ;
        } 
        size = ss.size() ;
        index = ss.find("\r\n") ;
        len = atoi(ss.substr(0, index).c_str()) ;
        ss = ss.substr(index+2, size-index-2) ;
        if(len == (int)ss.size()) {
            buffer+=ss ;
            break ;
        }
        buffer+=ss ;
    }   
    close(sockFd) ;
    return ss ;
}
