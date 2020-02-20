#include "CgiConnect.h"

int cgiConnect :: sockFd  ;
struct sockaddr_in cgiConnect:: addr  ;
char cgiConnect :: buf[BUFFERSIZE] = {'0'};

cgiConnect :: cgiConnect() {
}

int cgiConnect ::connectCgiServer() {
    sockFd = socket(AF_INET, SOCK_STREAM, 0) ;
    if(sockFd < 0) {
        std::cout << __LINE__ << "       " << __FILE__ << std::endl ;
        return -1 ;
    }
    addr.sin_family = AF_INET ;
    addr.sin_port = htons(PORT) ;
    addr.sin_addr.s_addr = inet_addr(IP) ;
    int ret = connect(sockFd, (struct sockaddr*)&addr, sizeof(addr)) ;
    if(ret < 0) {
        std::cout << __LINE__ << "   " << __FILE__ << std::endl ;
        return -1 ;
    }
    return sockFd ;
}

int cgiConnect :: sendMsg(const char*  buffer) {
    strcpy(buf, buffer) ;
    if(send(sockFd, buf, sizeof(buf), 0) < 0) {
        std::cout << __LINE__ << "     " << __FILE__ << std::endl ;
        return -1 ;
    }
    return 1 ;
}

std::string cgiConnect :: recvMsg() {
    std::string ss ;
    if(recv(sockFd, buf, sizeof(buf), 0)<0) {       
        std::cout << __LINE__ << "     " << __FILE__ << "     " << strerror(errno)<< std::endl ;
        return "" ;
    } 

    ss += buf ;
    return ss ;
}
