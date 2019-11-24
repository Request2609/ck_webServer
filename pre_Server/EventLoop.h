#pragma  once
#include <memory>
#include <atomic>
#include <iostream>
#include <queue> 
#include <thread>
#include <mutex> 
#include <map>
#include "ThreadPool.h"
#include "Channel.h"
#include "Epoll.h"
#include "Connection.h"
using namespace std ;
class epOperation ;
class channel ;
class connection ;
class eventLoop ;

class loopInfo {
    friend eventLoop ;
public :
    loopInfo() { 
        ep = make_shared<epOperation>() ;
    }
    ~loopInfo() {
        
    }
public :
    size_t size() { return chlList.size() ; }
    static void wakeCb(channel* chl); 
    void print() ;
    void add(int fd, shared_ptr<channel>chl) {
          chlList.insert(make_pair(fd, chl)) ; 
    }
    shared_ptr<channel> getChl() { return chl ; }
    int addConnect(channel* chl) ;
    void setId(int num) { id = num; } 
    int setChannel() ;
    shared_ptr<channel>search(int fd) ;
    shared_ptr<epOperation> getEp() { return ep ; }
    int buildWakeFd() ; 
    //返回所属线程唤醒句柄
    int getWriteFd() { return wakeupFd[0]; }
    //唤醒读事件
    int getReadFd() { return wakeupFd[1] ; }
    //返回所属线程epoll句柄
    //返回线程id
    static int delChl(int fd, map<int, shared_ptr<channel>>& tmp) ;
    void setThreadId(long id) { threadId = id ; }
    void clearChannel(int fd) ;
    int setNoBlock(int fd) ;
    int getId() { return id ; }
private :
    map<int, shared_ptr<channel>>chlList ;
    shared_ptr<channel>chl ;
    //事件循环中的epoll
    shared_ptr<epOperation>ep ;
    //唤醒线程的fd
    int wakeupFd[2] ;
    //线程id
    long threadId ; 
    int id ;
    //该loop管理的事件集合
} ;

//事件循环 EventLoop和connection共享channel对象
class eventLoop {   
    typedef std::map<int, channel> channelMap;
public:
    eventLoop() ;
    ~eventLoop() ;
public :
    int wakeup(int fd) ;
    vector<pair<int, channel>> doPendingFunc(shared_ptr<channel>chl) ;
    void runThread() ;
    channel* search(int fd) ;
    int getListenFd() { return servFd ; }
    void loop() ;
    void addConnection(connection* con) ;
    void addClList(int fd, channel& channel_) ;
    int fillChannelList(channel chl) ;
    channel  handleAccept() ;
    int clearCloseChannel(std::vector<channel>&list_) ;
    void round(loopInfo loop, shared_ptr<channel>, shared_ptr<epOperation>) ;
    int queueInLoop(channel chl, int& num) ;
    void addQueue(vector<pair<int, channel>>&ls, loopInfo&loop, shared_ptr<epOperation>ep) ;
    int getNum() ;
private :
    vector<promise<int>>pro ;
    shared_ptr<threadPool>pool ;
    shared_ptr<threadPool>pool1 ;
    int threadNums ;
    mutex mute ;
    //线程
    vector<thread> threads ;
    //线程loop中的重要信息
    vector<loopInfo> info ;
    int servFd  = -1 ;
    connection* conn ;
    //一个eventLoop一个epoll
    std :: shared_ptr<epOperation> epPtr ;
    //活跃事件列表
    std :: vector<channel> activeChannels;
    map<int, channel> qChl ;
    //该eventLoop对应的监听套接字封装
    //退出标志
    bool quit ;
    //设置套接字channel键值对
    channelMap clList ;
    int id ;
};

