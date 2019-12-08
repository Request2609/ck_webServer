#include "Epoll.h"

void epOperation :: add(int fd, int events) {
 //   cout << "添加可读事件: " << fd << endl ;
    struct epoll_event ev ;
    ev.data.fd = fd ;
    ev.events = events ;
    if(epoll_ctl(epFd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        std :: cout << __FILE__ << "   " << __LINE__ << "   " << strerror(errno)<< std :: endl ;
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
        std :: cout << __FILE__ << "   " << __LINE__ << std :: endl ;
        return ;
    }
}

void epOperation :: del(int fd) {
   // cout << "关闭链接" << endl ;
    if(epoll_ctl(epFd, EPOLL_CTL_DEL, fd, NULL) < 0){
    //    std :: cout << __FILE__ << "   " << __LINE__ << "      " << strerror(errno)<< std :: endl ;
        return  ;
    }
   //cout << "关闭完成          " << fd<<endl ;
    fds -- ;
}
void epOperation :: del(int epFd, int fd) {
    if(epoll_ctl(epFd, EPOLL_CTL_DEL, fd, NULL) < 0){
   //     std :: cout << __FILE__ << "   " << __LINE__ << std :: endl ;
        return  ;
    }
}

//将活跃的事件全加入到clList
int epOperation :: wait(eventLoop* loop, int64_t timeout, int index, int listenFd) {
    
    int eventNum ;
    struct epoll_event epFd_[200] ;
    try{
        eventNum = epoll_wait(epFd, epFd_, 200, timeout) ;
    }catch(exception e) {
        cout << e.what() ;
    }
    if(eventNum < 0) {
        cout << eventNum << "           错误：" << strerror(errno) << endl ;
        return -1 ;
    }
    //将活跃件全部加入到事件列表中
    for(int i=0; i<eventNum; i++) {
        int fd = epFd_[i].data.fd ;
        //要是还未注册的事件
        if(fd == listenFd) {
            printf("123\n") ;
            loop->handleAccept(index, listenFd) ;
        }
        else {
    //cout << "触发可读事件" << "     " << index << "-------->" <<listenFd <<"<--------------------"  << fd<< endl ;
            shared_ptr<channel> chl = loop->search(index, fd) ;
            loop->fillChannelList(index, chl) ;
        }
    }
    return eventNum ;
}

