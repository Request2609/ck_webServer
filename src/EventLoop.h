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
#include "Socket.h"
#include "ObjectPool.h"
#include "ParseConf.h"
#include "Log.h"

class epOperation ;
class channel ;
class connection ;
class eventLoop ;
class socketFd ;

//事件循环 EventLoop和connection共享channel对象
class eventLoop {   
    //线程号码
    typedef std::vector<std::vector<std::pair<int, std::shared_ptr<channel>>>> channelMap;
    typedef std::function<void(channel* chl, 
                               std::vector<std::pair<int, std::shared_ptr<channel>>>&)> callBack ;
public:
    eventLoop() ;
    ~eventLoop() ;
public :
    int wakeup(int fd) ;
    void runThread() ;
    std::shared_ptr<channel> search(int index, int fd) ;
    int getListenFd() { 
        return servFd ; 
    }
    void loop() ;
    void initObjectPool() ;
    void addConnection(connection* con) ;
    void addClList(int fd, std::shared_ptr<channel>& channel_) ;
    int fillChannelList(int index, std::shared_ptr<channel>chl) ;
    std::shared_ptr<channel>  handleAccept(int index, int listenFd) ;
    int clearCloseChannel(int fd, int index) ;
    void round(std::shared_ptr<channel>, std::shared_ptr<epOperation>) ;
    int getNum() ;
    void initEventSet() ;   
    void printClList() ;
    channelMap getMap() { return clList ;}
    std::shared_ptr<socketFd> getSock() ;
    static std::mutex mute ;
private :
    callBack readCall ;
    //对象池
    std::shared_ptr<objectPool<channel>> obp ;
    //线程池
    std::shared_ptr<threadPool>pool ;
    //ep对象
    std :: shared_ptr<epOperation> epPtr ;

    int objectNum ;
    std::string ip ;
    std::string port ;
    int threadNums ;
    //线程
    //线程loop中的重要信息
    int servFd  = -1 ;
    connection* conn ;
    //一个eventLoop一个epoll
    //活跃事件列表，二维数组
    std::vector<std::vector<std::shared_ptr<channel>>> activeChannels ;
    std::map<int, std::shared_ptr<epOperation>> epSet ;
    //该eventLoop对应的监听套接字封装
    //退出标志
    bool quit ;
    //设置套接字channel键值对
    channelMap clList ;
    int id ;
    std::shared_ptr<log>err ;
};


