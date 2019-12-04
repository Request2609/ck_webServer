#include "TcpServer.h"

//根据连接端口给服务命名
tcpServer :: tcpServer(eventLoop* loop, std::string  port) {
    //获取用户传进来的eventLoop
    this->loop = loop ;
    this->port = atoi(port.c_str()) ;
    //设置线程先设置一个
}

void tcpServer :: create(eventLoop* loop) {
    this->loop = loop  ;
}

void tcpServer :: create(eventLoop* loop, std::string  port) {
    //获取用户传进来的eventLoop
    this->loop = loop ;
    this->port = atoi(port.c_str()) ;
    //设置线程先设置一个
}

//传一个参数的构造
tcpServer :: tcpServer(eventLoop* loop) {
    this->loop = loop ;
    return ;       
}

//用户只需要设置好conn的各种回调函数
//tcpServer
void tcpServer :: addNewConnection(connection* conn) {
    this->conn = conn ;
    std::shared_ptr<socketFd> sock = this->conn->getSock() ;
    //创建监听套接字，并设置好内部channel类
    if(sock->getBindAddr() == 1) {
        this->conn->createListenFd(&(*sock)) ;
        return ;
    }
    this->conn->createListenFd(port) ;
}
//开始
void tcpServer :: start() {
    //将新连接加入到loop中，暂时单线程，可以开线程
    loop->addConnection(conn) ; 
}

