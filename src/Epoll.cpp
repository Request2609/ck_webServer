#include "Epoll.h"

void epOperation :: add(int fd, int events) {
    struct epoll_event ev ;
    ev.data.fd = fd ;
    ev.events = events ;
    if(epoll_ctl(epFd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
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
        std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        return ;
    }
}

void epOperation :: del(int fd) {
    if(epoll_ctl(epFd, EPOLL_CTL_DEL, fd, NULL) < 0){
        //string s = to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno) ;
        //(*err)<<s ;
        return  ;
    }
    fds -- ;
}
void epOperation :: del(int epFd, int fd) {
    if(epoll_ctl(epFd, EPOLL_CTL_DEL, fd, NULL) < 0){
        std :: cout << __FILE__ << "   " << __LINE__ << std :: endl ;
        return  ;
    }
}

//将活跃的事件全加入到clList
int epOperation :: wait(eventLoop* loop, int64_t timeout, int index, int listenFd) {
    int eventNum ;
    struct epoll_event epFd_[200] ;
    try{
        eventNum = epoll_wait(epFd, epFd_, 200, timeout) ;
    }catch(std::exception e) {
        std::cout << e.what() ;
    }
    if(eventNum < 0) {
        std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        return -1 ;
    }
    //将活跃件全部加入到事件列表中
    for(int i=0; i<eventNum; i++) {
        int fd = epFd_[i].data.fd ;
        //要是还未注册的事件
        if(fd == listenFd) {
            loop->handleAccept(index, listenFd) ;
        }
        else {
            std::shared_ptr<channel> chl = loop->search(index, fd) ;
            loop->fillChannelList(index, chl) ;
        }
    }
    return eventNum ;
}

int epOperation::getPort(int fd) {

    struct sockaddr_in sock ;
    socklen_t len ;
    len=sizeof(sock) ;
    getpeername(fd, (struct sockaddr*)&sock, &len) ;
    int port = ntohs(sock.sin_port)  ;
    return port ;
}


