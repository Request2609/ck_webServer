#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_
#include <map>
#include<functional>
#include<stdlib.h>
#include "ThreadPool.h"
#include "Connection.h"
#include "EventLoop.h"

class tcpServer {
    //将连接名置为端口号
    typedef std::map<std::string, connection> connMap ;
public:
    //暂时使用回环网卡测试，就不传IP了！
    tcpServer(eventLoop* loop, std::string port) ;
    tcpServer(eventLoop* loop) ;
    tcpServer(): loop(nullptr) {}
    ~tcpServer() {}
public :
    void create(eventLoop* loop) ;
    void create(eventLoop* loop, std::string port) ;
    void  addNewConnection(connection* conn) ;
    void start() ;
private :
    //事件循环
    eventLoop* loop ;
    //连接名称
    int port ;
    std::string addr ;
    //连接列表
    connection* conn ;
};
#endif

