#include"Connection.h"

using namespace std :: placeholders ;

connection :: connection() {
    readCallBack = nullptr ;
    closeCallBack = nullptr ;
    writeCallBack = nullptr ;
    timeoutCallBack = nullptr ;
    //创建一个套接字新连接
    buffer.bufferClear() ;
}

void connection::getChannel(channel* chl) {
    *chl = *channel_ ;
}

void connection :: setConf(std::string ip, std::string port) {
    readCallBack = nullptr ;
    closeCallBack = nullptr ;
    writeCallBack = nullptr ;
    timeoutCallBack = nullptr ;
    //创建一个套接字新连接

    channel_ = std :: make_shared<channel>() ;
    //绑定了ip和端口已经绑定
    sock = std :: make_shared<socketFd>(ip, port) ;   
    //设置号连接套接字的端口和IP
    sock->setBindAddr(1) ;
    buffer.bufferClear() ;
}

//创建指定ip和端口的套接字
connection :: connection(std::string ip, std::string port) {
    readCallBack = nullptr ;
    closeCallBack = nullptr ;
    writeCallBack = nullptr ;
    timeoutCallBack = nullptr ;
    //创建一个套接字新连接
    channel_ = std :: make_shared<channel>() ;
    sock = std :: make_shared<socketFd>(ip.c_str(), port.c_str()) ;   
    buffer.bufferClear() ;
}

connection :: ~connection() {
}

void connection :: setWakeChlCall(shared_ptr<channel>&chl) { 
    chl->setWakeCall(wakeCb);
}

//给channel设置回调
void connection :: setCallBackToChannel(shared_ptr<channel> channel_) {
    int flag = 0 ;
    if(readCallBack != nullptr) {
        flag = 1 ;
        channel_->setReadCallBack(readCallBack) ;
    }
    if(writeCallBack != nullptr) {
        flag = 2 ;
        channel_->setWriteCallBack(writeCallBack) ;
    }
    if(timeoutCallBack != nullptr) {
        flag = 3 ;
        channel_->setTimeoutCallBack(timeoutCallBack) ;
    }
    if(closeCallBack != nullptr) {
        flag = 4  ;
        channel_->setCloseCallBack(closeCallBack) ;
    }
}

//关闭连接
void connection :: shutdown() {
    int connFd = sock->getAcceptSock() ;
    sock->shutdownWrite(connFd) ;
}

void connection :: setReadCallBack(callBack cb) {

    channel_->setReadCallBack(cb) ;
    readCallBack = std::move(cb) ;
}

void connection :: setCloseCallBack(callBack cb) {
    channel_->setCloseCallBack(cb) ;
    closeCallBack = cb ;
}

void connection :: setWriteCallBack(callBack cb) {
    channel_->setWriteCallBack(cb) ;
    writeCallBack = cb ;
}

void connection :: setTimeoutCallBack(callBack cb) {
    channel_->setTimeoutCallBack(cb) ;
    timeoutCallBack = cb ;
}


//与下面的函数少了一步绑定地址
void connection :: createListenFd(socketFd* sock) {
   // sock->setNoBlocking(sock->getListenSock());
    //sock->setReuseAddr() ;
    sock->startListen() ;
    int fd = sock->getListenSock() ;
    channel_->setFd(fd) ;
}

//创建一个新监听套接字，并将相应的channel中的fd设置一下
int connection :: createListenFd(int port) {
    sock->setAddr(port) ;   
   /// sock->setNoBlocking(sock->getListenSock()) ;
    //sock->setReuseAddr() ;
    sock->setReusePort() ;
    sock->bindAddress() ;
    sock->startListen() ;
    int fd = sock->getListenSock() ;
    channel_->setFd(fd) ;
    return 1 ;
} ;

//接收tcp新连接，先判断新连接的监听套接字存不存在
int connection :: doAccept() {
    int servFd = sock->getListenSock() ;
    if(servFd == -1) {
        std::cout << __FILE__ << "    " << __LINE__ << std::endl ;  
        return -1 ;
    }   
    //存在的话
    int connFd = sock->acceptSocket() ;
    //接受连接失败;
    if(connFd < 0) {
        std::cout << __FILE__ << "    " << __LINE__ << std::endl ;  
        return -1 ;
    }
    this->fd = connFd ;
    return 1 ;
}   


