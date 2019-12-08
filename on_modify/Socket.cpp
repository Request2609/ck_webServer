#include "Socket.h"

socketFd :: socketFd() {
    sockFd = socket(AF_INET, SOCK_STREAM, 0) ;
    if(sockFd < 0) {
        std::cout<<__FILE__<<__LINE__<<std::endl; ;
        return ;
    }
    setLinger(sockFd) ;
}

socketFd :: socketFd(int port) {
    this->port = to_string(port) ;
    sockFd = socket(AF_INET, SOCK_STREAM, 0) ;
    if(sockFd < 0) {
        std::cout<<__FILE__<<__LINE__<<std::endl; ;
        return ;
    }
    setLinger(sockFd) ;
    bzero(&sockAddr, sizeof(sockAddr)) ;
    sockAddr.sin_family = AF_INET ;
    sockAddr.sin_port = htons(port) ;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY) ;
    this->port = port ; 
}

socketFd :: socketFd(const char* port) {
    this->port = port ;
    sockFd = socket(AF_INET, SOCK_STREAM, 0) ;
    if(sockFd < 0) {
        std::cout<<__FILE__<<__LINE__<<std::endl; ;
        return ;
    }
    setLinger(sockFd) ;
    bzero(&sockAddr, sizeof(sockAddr)) ;
    sockAddr.sin_family = AF_INET ;
    sockAddr.sin_port = htons(atoi(port)) ;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY) ;
    this->ip = "127.0.0.1" ;
}

socketFd :: socketFd(const std::string addr, const std::string port) {
    ip = addr ;
    this->port = port ;
    sockFd = socket(AF_INET, SOCK_STREAM, 0) ;
    if(sockFd < 0) {
        std::cout<<__FILE__<<__LINE__<<std::endl; ;
        return ;
    }
    setLinger(sockFd) ;
    bzero(&sockAddr, sizeof(sockAddr)) ;
    sockAddr.sin_family = AF_INET ;
    sockAddr.sin_port = htons(atoi(port.c_str())) ;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY) ;
    setReusePort() ;
    if(bind(sockFd, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0) {
        std::cout << __FILE__ << "       " <<__LINE__ << "      " << strerror(errno) <<std::endl ;
        return ;
    }
}   

int socketFd :: setAddr(int port) {
    bzero(&sockAddr, sizeof(sockAddr)) ;
    this->port = to_string(port) ;
    sockAddr.sin_family = AF_INET ;
    sockAddr.sin_port = htons(port) ;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY) ;
    return 1 ;
}

//设置地址
int socketFd :: setAddr(const char* ip, const char* port) {
    assert(ip != NULL) ;
    assert(port != NULL) ;
    this->port = port ;
    bzero(&sockAddr, sizeof(sockAddr)) ;
    sockAddr.sin_family = AF_INET ;
    sockAddr.sin_port = htons(atoi(port)) ;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY) ;
    this->ip = ip ;
    this->port = port ;
    return 1 ;
}

int socketFd :: setAddr(const char* port) {
    this->ip = "127.0.0.1" ;
    this->port = port ;
    assert(port != NULL) ;
    bzero(&sockAddr, sizeof(sockAddr)) ;
    sockAddr.sin_family = AF_INET ;
    sockAddr.sin_port = htons(atoi(port)) ;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY) ;
    return 1 ;
}

void socketFd :: shutdownWrite(int fd) {
    int ret = shutdown(fd, SHUT_WR) ;
    if(ret < 0) {
        std::cout<<__FILE__<<__LINE__<<std::endl; ;
        return ;
    }
}

int socketFd :: getListenSock() {
    return sockFd ;
}

int socketFd :: getAcceptSock() {
    return connFd ;
}

//设置客户端延迟关闭选项
int socketFd :: setLinger(int fd) {
    struct linger so_linger ;
    //开启选项
    so_linger.l_onoff = 0;
    so_linger.l_linger = 0 ;
    int ret = setsockopt(fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger)) ;
    if(ret < 0) {
        std::cout << __FILE__<< "          " <<__LINE__ << std::endl ;
        return -1 ;
    }
    return  1 ;
}   

int socketFd :: setReuseAddr() {
    int flag = 1 ;
    if(sockFd < 0) {
        std::cout<<__FILE__<<__LINE__<<std::endl; ;
        return -1;;
    }
    int ret = setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) ;
    if(ret < 0) {
        std::cout<<__FILE__<<__LINE__<<std::endl; ;
        return -1;
    }
    return 1 ;
}

int socketFd :: setReusePort() {    
    int flag = 1;
    int ret = setsockopt(sockFd, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag)) ;
    if(ret < 0) {
        std::cout << __FILE__ << "       " << __LINE__ << std::endl ;
        return -1 ;
    }
    return 1 ;
}


int socketFd :: bindAddress() {
    assert(sockFd != -1) ;
    if(bind(sockFd, (struct sockaddr*)&sockAddr, sizeof(sockAddr))< 0) {
        std::cout<<__FILE__<<__LINE__<<"        "<< strerror(errno)<<std::endl; ;
        return -1 ;
    }
    return 1 ;
}    

int socketFd :: acceptSocket() {

    assert(sockFd != -1) ;
   //cout << "接受连接----------------------------------------->" << sockFd << endl ;
    int connFd = accept(sockFd, NULL, NULL) ;
    if(connFd < 0) {
        std::cout<<__FILE__<<"       " << __LINE__ << "      " << strerror(errno)<<std::endl; ;
        return -1 ;
    }
    this -> connFd = connFd ;
    return connFd ;
}

int socketFd :: startListen() {
    assert(sockFd != -1) ;
    if(listen(sockFd, BACKLOG) < 0) {
        std::cout<<__FILE__<<__LINE__<<std::endl; ;
        return -1 ;
    }
    return 1 ;
}

int socketFd :: setNoBlocking(int fd) {
    int old = fcntl(fd, F_GETFL, 0) ;
    assert(old != -1) ;
    int newA = old|O_NONBLOCK ;
    int ret = fcntl(fd, F_SETFL, newA) ;
    if(ret < 0) {
        std::cout<<__FILE__<<__LINE__<<std::endl; ;
        return -1 ;
    }
    return 1 ;
}


