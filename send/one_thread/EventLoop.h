#pragma  once
#include <memory>
#include <atomic>
#include <iostream>
#include <map>
#include "Channel.h"
#include "Epoll.h"
#include "Connection.h"

class channel ;
class connection ;
class epOperation ;
//事件循环 EventLoop和connection共享channel对象
class eventLoop
{   
    typedef std::map<int, channel> channelMap;
public:
    eventLoop() ;
    ~eventLoop() {}
public :
    channel* search(int fd) ;
    int getListenFd() { return servFd ; }
    void loop() ;
    void addConnection(connection* con) ;
    void addClList(int fd, channel& channel_) ;
    int fillChannelList(channel*chl) ;
    void handleAccept() ;
    int clearCloseChannel(std::vector<channel>&list_) ;
private:
    int servFd  = -1 ;
    connection* conn ;
    //reactor in thread
    //一个eventLoop一个epoll
    std :: shared_ptr<epOperation> epPtr ;
    //活跃事件列表
    std :: vector<channel> activeChannels;
    //该eventLoop对应的监听套接字封装
    //退出标志
    bool quit ;
    //设置套接字channel键值对
    channelMap clList ;
};

