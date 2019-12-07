#pragma  once
#include <memory>
#include <atomic>
#include <iostream>
#include <map>
#include <vector>
#include "Channel.h"
#include "Epoll.h"
#include "Connection.h"
#include "ThreadPool.h"

using namespace std ;
class channel ;
class connection ;
class epOperation ;
//事件循环 EventLoop和connection共享channel对象
class eventLoop
{   
    typedef std::vector<pair<int, shared_ptr<channel>>> channelMap;
public:
    eventLoop() ;
    ~eventLoop() {}
public :
    shared_ptr<channel> search(int fd) ;
    int getListenFd() { return servFd ; }
    void loop() ;
    void addConnection(connection* con) ;
    void addClList(int fd, channel& channel_) ;
    int fillChannelList(shared_ptr<channel> chl) ;
    void handleAccept() ;
    int clearCloseChannel(std::vector<channel>&list_) ;
    void processConnect() ;
    static void threadTask(shared_ptr<channel> chl, vector<pair<int, shared_ptr<channel>>>&) ;
    static void closeConnect(shared_ptr<channel>chl, vector<pair<int, shared_ptr<channel>>>& clList) ;
    static mutex mute ;
private:
    int servFd  = -1 ;
    connection* conn ;
    //reactor in thread
    //一个eventLoop一个epoll
    std :: shared_ptr<epOperation> epPtr ;
    //活跃事件列表
    std :: vector<shared_ptr<channel>> activeChannels;
    shared_ptr<threadPool> pool ;
    //该eventLoop对应的监听套接字封装
    //退出标志
    bool quit ;
    //设置套接字channel键值对
    channelMap clList ;
};

