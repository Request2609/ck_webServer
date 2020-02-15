#include "Channel.h"

const int SIZE = 4096 ;
channel :: channel() {
    events = 0 ;
    input.bufferClear() ;
    output.bufferClear() ;
    cliFd = -1 ;
    len = 0 ;
}

//接收新连接
int channel::handleAccept(int servFd) {
    //设置监听套接字
    
    sock->setListenFd(servFd) ;
    //获取新客户端连接
    cliFd = sock->acceptSocket() ;
    sock->setNoBlocking(cliFd) ;
    setEvents(READ) ;
    return cliFd ;
}

//修改监听事件
int channel :: updateChannel() {
    struct epoll_event ev ;
    ev.data.fd = cliFd ;
    ev.events = events ;
    int ret = epoll_ctl(epFd, EPOLL_CTL_MOD, cliFd, &ev) ;
    if(ret < 0) {
        std::cout << __FILE__ << "      " << __LINE__ << std::endl ;
        return -1 ;
    } 
    return 1 ;
}


void channel::clearBuffer() {
    input.bufferClear() ;
    output.bufferClear() ;
}


bool channel :: operator==(channel& chl) {
    if(cliFd == chl.getFd()) {
        return 1 ;
    }
    return 0 ;
}

int channel :: handleEvent(int fd, vector<pair<int, shared_ptr<channel>>>& tmp, int index) {    
    if(events&EPOLLIN) {
        int n = handleRead(tmp) ;
        if(n < 0) {
            return -1;
        }
        if(n == 0) {
            for(auto s = tmp.begin(); s!=tmp.end(); s++) {
                if(s->first == fd) {
                    //归还对象
                    auto obp = objectPool<channel>::getPool() ;
                    obp->returnObject(s->second, index) ;
                    tmp.erase(s) ;
                    break ;
                }
            }
            ep->del(fd) ;
            close(fd) ;
            return 0 ;
        }
    }

    if(events&EPOLLOUT) {
        //发送数据
        handleWrite() ;
        return 0 ;
    }
    return 1 ;
}


void channel :: delFd(int fd, map<int, shared_ptr<channel>>& tmp) {
    auto ret = tmp.find(fd) ;
    if(ret == tmp.end()) {
        return ;
    }
    tmp.erase(ret) ;
}

//执行写回调
int channel :: handleWrite() {
    //写缓冲区
    char buf[SIZE] ;
    int index = 0;
    string s = "" ;
    bzero(buf, sizeof(buf)) ;
    int len = output.getSize() ;
    for(int i=0; i<len; i++) {
        buf[index] = output[i] ;
        index++ ;
        if(!(index%(SIZE-2))||i==len-1) {
            buf[index] = '\0' ;
            int ret = writen(cliFd, s.data(), index+1) ;
            if(ret < 0) {
                cout << __LINE__ << "       " << __FILE__ <<"     " << strerror(errno)<< endl ;
                return -1 ;
            }
            if(i == len-1) break ;
            bzero(buf, sizeof(buf)) ;
            index = 0 ;
            s.clear() ;
        }
    }
    return 0 ;
}

//执行读回调
int channel :: handleRead(vector<pair<int, shared_ptr<channel>>>&tmp) {
    //读数据
    int n = input.readBuffer(cliFd) ;
    if(n < 0) {
        return -1 ;
    }
    ///读到0字节数据表明对端已经关闭连接
    if(n == 0) {
        return 0 ;
    }
    //消息设置好后，调用用户回调函数处理    
    if(input.getCanProcess() == true) {  
        readCallBack(this, tmp) ;
    }
    return 1 ;
}
