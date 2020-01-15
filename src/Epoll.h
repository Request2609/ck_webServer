#ifndef _EPOLL_H_
#define _EPOLL_H_
#include <sys/epoll.h>
#include <vector>
#include <unistd.h>
#include "Channel.h"
#include "EventLoop.h"
using namespace std ;

class channel ;
class loopInfo ;
class eventLoop ;
class epOperation {
public :
    epOperation():fds(0),nfds(200) {
        epFd = epoll_create(1) ;
       //设置了epfds的capacity大小
        epFds.reserve(nfds) ;
    }   

    ~epOperation() { close(epFd) ; }
    static void del(int epFd, int fd) ;
public :
    int getEpFd() {return epFd ;}
    int  wait(eventLoop* loop, int64_t timeout, int index, int listenFd) ;
    void add(int fd, int events) ;
    void change(int fd, int events) ;
    void del(int fd) ;
private :
    int epFd ;
    //帮助更新epoll中的事件数量
    int fds ;
    int nfds ;
    std :: vector<struct epoll_event> epFds ;
};
#endif
