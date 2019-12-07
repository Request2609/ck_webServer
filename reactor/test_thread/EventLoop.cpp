#include"EventLoop.h" 

mutex eventLoop:: mute ;

eventLoop :: eventLoop() {
    //创建一个epoll
    epPtr = std::make_shared<epOperation>() ;
    quit = false ;
    pool = make_shared<threadPool>(8) ;
}


void eventLoop :: closeConnect(shared_ptr<channel>chl, vector<pair<int, shared_ptr<channel>>>& clList) {
    lock_guard<mutex> lk(mute) ;
    int fds = std::move(chl->getFd()) ;
    chl->getEp()->del(fds) ;
    //从map列表中找到并删除
    for(auto s=clList.begin(); s != clList.end(); s++) {
        if(s->first == fds) {
            clList.erase(s) ;
            break ;
        }
    }
    close(fds) ;
}

void eventLoop :: loop() {
    //将conn加入到epoll中 
    int events = clList[0].second->getEvents() ;
    //将当前的服务加入到epoll中
    epPtr->add(servFd, events) ;
    while(!quit) {
        //等待事件
        int ret = epPtr->wait(this, -1) ;
        if(ret < 0) {
            quit = true ;
        }
        //epoll返回０，没有活跃事件
        else if(ret == 0 || activeChannels.size() == 0) {
            continue ;
        }
        else {
            processConnect() ;
            //清空活跃事件集合
        }
    }   
}


void eventLoop :: threadTask(shared_ptr<channel>chl, vector<pair<int, shared_ptr<channel>>>&clList) {

    int n = chl->handleEvent(clList) ;
    //n==0就关掉该链接
    if(n == 0) {
        //返回0,关闭连接
        closeConnect(chl, clList) ;
    }

    else if((errno == EAGAIN && n == -1)){
        //重新加到Epoll中
        chl->getEp()->change(chl->getFd(), READ|EPOLLONESHOT) ;
    }
    
}

void eventLoop::processConnect() {
    auto task = bind(&eventLoop::threadTask, placeholders::_1, placeholders::_2) ;
   // cout << "当前活跃列表数量:" << activeChannels.size() << endl ;
    int len = activeChannels.size() ;
    for(int i=0; i<len ;i++) {
       // cout << "分发fd:" << activeChannels[i].getFd() << endl ;
        pool->commit(task, activeChannels[i], ref(clList)) ;
    }
    activeChannels.clear() ;
}   

//将活跃的事件加入到活跃事件表中
int eventLoop :: fillChannelList(shared_ptr<channel> chl) {
   activeChannels.push_back(chl) ;
   return 1 ;
}

//值有监听套接字拥有connection
//向loop中添加新的conn.将这个连接加入到epoll中

void eventLoop :: addConnection(connection* con) {
    conn = con ;
    std::shared_ptr<channel> channel_ = conn->getChannel() ;
    int fd = channel_->getFd() ;
    servFd = fd ;
    channel_->setEp(epPtr) ;
    //将这个服务器监听套接字加入到epoll中，只负责监听可读事件，LT模式
    channel_->setEvents(READ) ;
    //将fd和channel作为键值存入channelMap中
    clList.push_back({fd, channel_}) ;
}


//根据描述符搜索channel对象
shared_ptr<channel> eventLoop :: search(int fd) {
    for(auto s : clList) {
        if(s.first == fd) {
            return s.second ;
        }
    }
    return nullptr ;
} 

//接收连接并添加channel
void eventLoop :: handleAccept() {

    shared_ptr<channel>tmp = make_shared<channel>() ;
    tmp->setSock(conn->getSock()) ;
    //创建新连接
    int conFd = tmp->handleAccept(servFd) ;
    tmp->setFd(conFd) ;
    //为channel设置回调
    //设置套接字非阻塞
    conn->setnoBlocking(conFd) ;
    tmp->setEp(epPtr) ;
    tmp->setEvents(READ) ;
    conn->setCallBackToChannel(tmp) ;
    lock_guard<mutex>lk(mute) ;
    epPtr->add(conFd, READ|EPOLLONESHOT) ;
    //将channel加入到当前loop的列表中
    clList.push_back({conFd, tmp}) ;
}
