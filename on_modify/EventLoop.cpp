#include"EventLoop.h" 
#include "Call.h"

eventLoop :: eventLoop() {
    //开8线程
    //创建一个epoll
    threadNums = 0 ;
    quit = false ;
    epPtr = std::make_shared<epOperation>() ;
    for(int i=0; i<=threadNums; i++) {
        vector<pair<int, shared_ptr<channel>>>ls ;
        clList.push_back(ls) ;
    }
    //初始化epoll事件集合
    for(int i=0; i<=threadNums; i++) {
        vector<shared_ptr<channel>> ls ;
        activeChannels.push_back(ls) ;
    }
    epSet[0] = epPtr ;
}

void eventLoop :: initEventSet() {
    ///初始化所有事件集合

}

eventLoop:: ~eventLoop() {
}

int eventLoop :: clearCloseChannel(int index, int fd) {
    //从epoll中删除套接字
    auto ret = clList[index] ;
    for(auto s=ret.begin(); s!=ret.end(); s++) {
        if(s->first == fd) {
            ret.erase(s) ;
            break ;
        }
    }
    epSet[index]->del(fd) ;
    close(fd) ;
    return 1 ;
}

shared_ptr<socketFd> eventLoop :: getSock() {
    shared_ptr<socketFd>sockFd = make_shared<socketFd>(ip.c_str(), port.c_str()) ;
    return sockFd ;
}
//创建线程
void eventLoop :: runThread() {

    for(int i=0; i<threadNums; i++) {
        //创建套接字，并且绑定地址
        shared_ptr<socketFd> sfd = getSock() ;
        shared_ptr<epOperation> ep = make_shared<epOperation>() ;
        shared_ptr<channel> chan = make_shared<channel>() ;
        chan->setSock(sfd) ;
        chan->setEp(ep) ;
        chan->setEpFd(ep->getEpFd()) ;
        int fd = sfd->getListenFd() ;
        chan->setId(i+1) ;
        epSet[i+1] = ep ;
        //开始监听
        sfd->startListen() ;
        auto func = bind(&eventLoop::round, this, placeholders::_1, placeholders::_2, placeholders::_3) ;
        pool->commit(func, chan, ep, fd) ;
    }
}

int eventLoop :: wakeup(int fd) {
    static int  ret = 0  ;
    ret++ ;
    int res = send(fd, &ret, sizeof(ret), 0) ;
    if(res < 0) {
        cout << __FILE__ << "         " << __LINE__ <<"   " << strerror(errno)<< endl ;
        return -1 ;
    }
    return 1 ;
}


//绑定匿名unix套接字
void eventLoop :: round(shared_ptr<channel>chl, shared_ptr<epOperation> ep, int fd) {

    int stop = 0 ;
    //获取epoll
    if(chl == nullptr) {
        return ;
    }
    chl->setFd(fd) ;
    int id = chl->getId() ;
    //将事件加到epoll
    ep->add(chl->getFd(), READ) ;
    //为唤醒描述符添加id号码
    chl->setId(chl->getId()) ;
    //将当前唤醒的channel加入到list中
    vector<shared_ptr<channel>> closeLst ;
    clList[id].push_back({fd, chl}); 
    //将wakeFd加入到epoll中
    while(!stop) {
        int ret = ep->wait(this, -1, id, fd) ;    
        if(ret < 0) {
            stop = true  ;
        }
        if(ret == 0) {
            continue ;
        }
        for(shared_ptr<channel> chl : activeChannels[id]) {
            int fd = chl->getFd() ;
            int ret = chl->handleEvent(fd, clList[id])  ;      
            if(ret == 0) {
                closeLst.push_back(chl) ;    
            }
        }
        for(auto chl : closeLst) {
            clearCloseChannel(id, chl->getFd()) ;  
        }
        closeLst.clear() ;
        activeChannels[id].clear() ;
    }
}


void eventLoop :: loop() {
    //创建线程0的epoll
    //创建线程池
    pool = make_shared<threadPool>(threadNums);
    //初始化时间列表
    //开始跑线程
    runThread() ;
    //将conn加入到epoll中 
    int events = clList[0][0].second->getEvents() ;
    //将当前的服务加入到epoll中
    epPtr->add(servFd, events) ;
    vector<shared_ptr<channel>>closeLst ;
    while(!quit) {
        //等待事件
        int ret = epPtr->wait(this, -1, 0, servFd) ;
        if(ret < 0) {
            quit = true ;
        }
        //epoll返回０，没有活跃事件
        else if(ret == 0) {
            continue ;
        }
        //将事件分发给各个线程
        else {
            //处理连接，所有连接事件分给各个线程中的reactor
            for(shared_ptr<channel> chl : activeChannels[0]) {
                int fd = chl->getFd() ;
                int ret = chl->handleEvent(fd, clList[0]) ;
                if(ret < 0) {
                    closeLst.push_back(chl) ;     
                }
            }
            for(auto chl : closeLst) {
                clearCloseChannel(0, chl->getFd()) ;
            }
            //清空活跃事件集合
            activeChannels[0].clear() ;
        }
    }   
}

//将活跃的事件加入到活跃事件表中
int eventLoop :: fillChannelList(int index, shared_ptr<channel>chl) {
   activeChannels[index].push_back(chl) ;
   return 1 ;
}

//值有监听套接字拥有connection
//向loop中添加新的conn.将这个连接加入到epoll中
void eventLoop :: addConnection(connection* con) {
       
    conn = con ;
    std::shared_ptr<channel> chl = con->getChannel() ;  
    int fd = con->getSock()->getListenSock() ;
    ip = con->getSock()->getIp() ;
    port = con->getSock()->getPort() ;
    servFd = fd ;
    //将这个服务器监听套接字加入到epoll中，只负责监听可读事件，LT模式
    chl->setFd(fd) ;
    chl->setEp(epPtr) ;
    chl->setEpFd(epPtr->getEpFd()) ;
    chl->setEvents(READ) ;
    clList[0].push_back({fd, chl}) ;
    //将fd和channel作为键值存入channelMap中
    //将这个服务器监听套接字加入到epoll中，只负责监听可读事件，LT模式
}

shared_ptr<channel> eventLoop :: search(int index, int fd) {
    
    auto it = clList[index] ;
    for(auto s=it.begin(); s!=it.end(); s++) {
        if(fd == s->first) {
            return s->second ; 
        }
    }
    return nullptr ;
} 

//接收连接并添加channel
shared_ptr<channel> eventLoop :: handleAccept(int index, int listenFd) {
    shared_ptr<channel> tmp = make_shared<channel>() ;
    tmp->setSock(conn->getSock()) ;
    //创建新连接
    int conFd = tmp->handleAccept(listenFd) ;
    tmp->setFd(conFd) ;
    //为channel设置回调
    //设置套接字非阻塞
    conn->setnoBlocking(conFd) ;
    //设置监听事件类型
    tmp->setEvents(READ) ;
    tmp->setId(index) ;
    tmp->setEp(epSet[index]) ;
    tmp->setEpFd(epSet[index]->getEpFd()) ;
    epSet[index]->add(conFd, READ) ;
    //cout << "添加成功!     " << conFd << endl ;
    //给channel设置回调函数
    conn->setCallBackToChannel(tmp) ; 
    clList[index].push_back({conFd, tmp}) ;
    //将channel加入到当前loop的列表中
    return tmp ;
}

