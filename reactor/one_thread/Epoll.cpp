#include "Epoll.h"

void epOperation :: add(int fd, int events) {

    struct epoll_event ev ;
    ev.data.fd = fd ;
    ev.events = events ;
    if(epoll_ctl(epFd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        std :: cout << __FILE__ << "   " << __LINE__ <<"      "<< strerror(errno)<< std :: endl ;
        return ;
    }

    if(++ fds > nfds) {
        nfds *= 2 ;
        epFds.reserve(nfds) ;
    }
}

//修改事件监听类型
void epOperation :: change(int fd, int events) {
    struct epoll_event ev ;
    ev.data.fd = fd ;
    ev.events = events ;
    if(epoll_ctl(epFd, EPOLL_CTL_MOD, fd, &ev) < 0) {
        std :: cout <<strerror(errno) <<"      " << __FILE__ << "   " << __LINE__ << std :: endl ;
        return ;
    }
}

void epOperation :: del(int fd) {
   // lock_guard<mutex>mutex(mute) ;
    if(epoll_ctl(epFd, EPOLL_CTL_DEL, fd, NULL) < 0){
        std :: cout << __FILE__ << "   " << __LINE__ << std :: endl ;
        return  ;
    }
    fds -- ;
}

//将活跃的事件全加入到clList
int epOperation :: wait(eventLoop* loop, int64_t timeout) {
    
    int listenFd = loop->getListenFd() ; 
    int eventNum = epoll_wait(epFd, &epFds[0], epFds.capacity(), timeout) ;
    //将活跃的事件全部加入到事件列表中
    for(int i=0; i<eventNum; i++) {
        int fd = epFds[i].data.fd ;
        //要是还未注册的事件
        if(fd == listenFd) {
            //接收并注册该连接
            loop->handleAccept() ;
            continue ;
        }
        //无论那种事件，否加入到活跃列表
        if(fd != listenFd) {
            channel* ch = loop->search(fd) ;
            if(ch == NULL) {
                continue ;
            }
            else {
                cout << "触发事件" << endl ;
                loop->fillChannelList(ch) ;   
            }
        }
    }
    return eventNum ;
}
