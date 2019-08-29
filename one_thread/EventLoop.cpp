#include"EventLoop.h" 

eventLoop :: eventLoop() {
    //创建一个epoll
    epPtr = std::make_shared<epOperation>() ;
    quit = false ;
}

int eventLoop :: clearCloseChannel(std::vector<channel>&list_) {
    //从epoll中删除套接字
    std::map<int, channel>::iterator iter ;
    for(channel chl:list_) {
        int fds = std::move(chl.getFd()) ;
        epPtr->del(fds) ;
        close(fds) ;
        //从map列表中找到并删除
        iter = clList.find(fds) ;
        if(iter == clList.end()) {
            return -1 ;
        }
        clList.erase(iter) ;
    }
    return 1 ;
}

void eventLoop :: loop() {
    //将conn加入到epoll中 
    int events = clList[servFd].getEvents() ;

    //将当前的服务加入到epoll中
    epPtr->add(servFd, events) ;
    while(!quit) {
        //等待事件
        int ret = epPtr->wait(this, -1) ;
        if(ret < 0) {
            quit = true ;
        }
        //epoll返回０，没有活跃事件
        else if(ret == 0) {
        }
        else {
            std::vector<channel>closeList ;
            //有事件发生
            for(channel channel: activeChannels) {
                //处理事件
                int n = channel.handleEvent() ;
                //将已经关闭的连接，收集起来
                if(n == 0) {
                    closeList.push_back(channel) ;
                }
            }
            int s = closeList.size() ;
            //从clList将关闭事件清除
            if(s != 0) {
                int n = clearCloseChannel(closeList) ;
                if(n < 0) {
                    std :: cout << __FILE__<< "    " << __LINE__<< std::endl ;
                }
            }
            //清空活跃事件集合
            activeChannels.clear() ;
        }
    }   
}

//将活跃的事件加入到活跃事件表中
int eventLoop :: fillChannelList(channel* chl) {
   activeChannels.push_back(*chl) ;
   return 1 ;
}

//值有监听套接字拥有connection
//向loop中添加新的conn.将这个连接加入到epoll中
void eventLoop :: addConnection(connection* con) {
    
    conn = con ;
    std::shared_ptr<channel> channel_ = conn->getChannel() ;
    int fd = channel_->getFd() ;
    servFd = fd ;
    channel_->setEpFd(epPtr->getEpFd()) ;
    //将这个服务器监听套接字加入到epoll中，只负责监听可读事件，LT模式
    channel_->setEvents(READ) ;
    //将fd和channel作为键值存入channelMap中
    clList[fd] = *channel_ ;
}

//根据描述符搜索channel对象
channel* eventLoop :: search(int fd) {

    std::map<int, channel> :: iterator it = clList.find(fd) ;
    if(it == clList.end()) {
        return NULL ;
    }
    else {
        return &(it->second) ;
    }
} 

//接收连接并添加channel
void eventLoop :: handleAccept() {
    channel tmp;
    tmp.setSock(conn->getSock()) ;
    //创建新连接
    int conFd = tmp.handleAccept(servFd) ;
    tmp.setFd(conFd) ;
    //为channel设置回调
    //设置套接字非阻塞
    conn->setnoBlocking(conFd) ;
    tmp.setEpFd(epPtr->getEpFd()) ;
    tmp.setEvents(READ) ;
    epPtr->add(conFd, READ) ;
    conn->setCallBackToChannel(&tmp) ;
    //将channel加入到当前loop的列表中
    clList[conFd] = tmp ;
}
