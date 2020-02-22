#pragma once
#include "component.h"

const int EPOLLNUM = 1024 ;

class process ;
class tool ;
class cgiConn ;

//进程池使用单实例模式
template<typename T>  
class processPool {
public :
    processPool(int listenFd, int processNum = 8) ;
    ~processPool() {}
public :
    static std::shared_ptr<processPool<T>> create(int listenFd, 
                                             int processNum = 8) ;
    static void sigHandle(int signo) ;
public :
    void run() ;
    void init() ;
private :
    //设置进程对应的管道
    //父进程运行
    void runParent() ;
    //子进程运行
    void runChild() ;
private :
    static const int maxProcessNum = 16 ; //进程池允许的最大子进程数量
    static const int userPreProcess = 65535 ;//每个进程最多能处理的客户数量
    static const int maxEventNum = 10000 ; //epoll最多处理的事件数量
    static std::shared_ptr<processPool<T>> object ; //进程池的静态实例 
    static int sigFd ;
    int curProcessNum ; //进程池目前能处理的进城数量
    int index ; //子进程在进程中的序号
    int epollFd ;  //epoll句柄
    int listenFd ; //监听的文件描述符
    bool stop ; //进程终止标记
    std::vector<std::shared_ptr<process>> proDescInfo ; // 保存进程的描述信息
};


template<typename T> 
int processPool<T>::sigFd = -1 ;

template<typename T> 
std::shared_ptr<processPool<T>> processPool<T> :: object = nullptr ;

//创建进程池
template<typename T>
processPool<T> :: processPool(int listenFd, int pNum)
    : listenFd(listenFd), curProcessNum(pNum), 
    index(-1), stop(false) {

    assert(pNum > 0 && pNum <= maxProcessNum) ;
    proDescInfo.reserve(pNum)  ;
    for(int i=0; i<pNum; i++) {
        std::shared_ptr<process> ptr ;
        proDescInfo.push_back(ptr) ;
    }
    for(int i=0; i<pNum; i++) {
        proDescInfo[i] = std::make_shared<process>() ;
    }

    //创建子进程
    for(int i=0; i<pNum; i++) {
        //创建unix双向匿名管道
        proDescInfo[i]->createSocketPair() ;
        proDescInfo[i]->pid = fork() ;
        assert(proDescInfo[i]->pid >= 0) ;
        //父进程
        if(proDescInfo[i]->pid > 0) {
            //关掉读端
            close(proDescInfo[i]->getReadFd()) ;
            continue ;
        }
        //子进程
        else {
            close(proDescInfo[i]->getWriteFd()) ;
            index = i ; //自己在进程池中的下标
            break ; //这里一定得跳出，否则子进程会继续创建进程
        }
    }
}

template <typename T>
std::shared_ptr<processPool<T>> processPool<T> :: create(int listenFd, 
                                                    int processNum) {
    if(object == nullptr) {
        object = std::make_shared<processPool<T>>(listenFd, processNum) ;
        object->init() ;
    }
    return object ;
}


//统一事件源
template<typename T>
void processPool<T>::sigHandle(int signo) {
    uint64_t count = signo ;   
    int ret = write(sigFd, &count, sizeof(count)) ;
    if(ret < 0) {
        std::cout << __FILE__ << "      " << __LINE__ << std::endl ;
        return ;
    }
    return ;
}

//初始化epoll，并统一事件源
template<typename T>
void processPool<T>::init() {
    epollFd = epoll_create(EPOLLNUM) ;
    if(epollFd < 0) {
        std::cout << __LINE__ << "       " << __FILE__ << std::endl; 
        return  ;
    }
    sigFd = tool::createEventFd() ;
    tool::addFd(epollFd, sigFd) ;
    signal(SIGCHLD, sigHandle) ;
    signal(SIGINT, sigHandle) ;
    signal(SIGTERM, sigHandle) ;
    signal(SIGPIPE, SIG_IGN) ;
}

//父进程在进程中的下标-1
template<typename T>
void processPool<T>::run() {
    if(index != -1) runChild() ;
    else runParent() ;
}

template<typename T> 
void processPool<T> :: runParent() {
    //设置信号
    tool :: addFd(epollFd, listenFd)  ; 
    epoll_event ev[maxEventNum] ;
    int count = 0 ;
    char newConn = 'c' ;
    while(!stop) {
        int number = epoll_wait(epollFd, ev, maxEventNum, -1) ;
        if(number < 0) {
            std::cout << __LINE__ << "      " << __FILE__ << std::endl ;
            return  ;
        }
        for(int i=0; i<number; i++) {
            int sockFd = ev[i].data.fd ;
            if(sockFd == listenFd) {
                int index = count ;
                do{
                    if(proDescInfo[index]->pid != -1) break ;
                    index = (index+1)%curProcessNum ;
                }while(index != count) ;

                if(proDescInfo[index]->pid == -1) { //没有子进程
                    stop = true ;
                    break ;
                }
                count = (index+1)%curProcessNum ;
                int ret = send(proDescInfo[index]->getWriteFd(), &newConn, sizeof(newConn), 0) ;
                if(ret < 0) {
                    std::cout << __LINE__ << "         " << __FILE__ << std::endl ;
                    return ;
                }
            }
            //信号事件，处理
            if(sockFd == sigFd) {
                if(ev[i].events&EPOLLIN) {
                    uint64_t count = 0 ;
                    read(sigFd, &count, sizeof(count)) ;
                    switch(count) {
                        case SIGCHLD :
                            pid_t pid ;
                            int stat ;
                            while((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
                                for(int i=0; i<proDescInfo.size(); i++) {
                                    close(proDescInfo[i]->getWriteFd()) ;
                                    proDescInfo[i]->pid = -1 ;
                                }
                            }
                            stop = true ;
                            //检查进程池子进程是否没有回收完
                            for(int i=0; i<proDescInfo.size(); i++) {
                                if(proDescInfo[i].pid != -1) {
                                    stop = false ;
                                    break ;
                                }
                            }
                            break ;
                        case SIGINT :
                        case SIGTERM :
                            for(int i=0; i<proDescInfo.size(); i++) {
                                int pid = proDescInfo[i].pid ;
                                if(pid != -1) {
                                    kill(pid, SIGTERM) ;
                                }
                            }
                            break ;
                    }
                }           
            }
        }   
    }
    close(epollFd) ;
}

//子进程的执行函数
template<typename T>
void processPool<T> :: runChild() {
    int pipeFd = proDescInfo[index]->getReadFd() ;
    //将管道描述符加入到epoll中
    tool::addFd(epollFd, pipeFd) ;
    epoll_event ev[maxEventNum]  ;
    //客户事件集合
    std::map<int, std::shared_ptr<T>> user ; 
    //为客户端设置处理的对象的池子
    while(!stop) {
        int num = epoll_wait(epollFd, ev, maxEventNum, -1) ;
        if(num < 0&&errno != EINTR) {
            std::cout << __LINE__ << "        " << __FILE__ << std::endl ;
            return ;
        } 
        for(int i=0; i<num; i++) {
            int fd =ev[i].data.fd ;
            if(fd == pipeFd&&(ev[i].events&EPOLLIN)) {
                char client ;
                int ret = recv(fd, &client, sizeof(client), 0) ;
                if(ret < 0) {
                    std::cout << __LINE__ << "      " << __FILE__ << std::endl ;
                    return ;
                }
                int connFd = accept(listenFd, NULL, NULL) ;
                if(connFd < 0) {
                    std::cout << __LINE__ << "       " << __FILE__ << std::endl ;
                    return ;
                }
                ret = tool::addFd(epollFd, connFd) ;
                if(ret < 0) {
                    std::cout << __LINE__ <<  "         " << __FILE__ << std::endl ;
                    return ;
                }
                //创建用户对象
                user[connFd] = std::make_shared<T>(epollFd, connFd) ;
            } 
            //处理事件
            else if(ev[i].events&EPOLLIN) {
                user[fd]->process() ;
            }

            //父进程接收到信号事件
            else if(fd == sigFd) {
                uint64_t count = 1 ;
                read(fd, &count, sizeof(count)) ;
                switch(count) {
                    case SIGINT :
                    case SIGTERM :
                        stop = true ;
                        break ;
                }
            }
        }
    }
    //回收资源
    if(!user.empty()) {
        for(auto res : user) {
            close(res.first) ;
        }
    }
    close(sigFd) ;
    close(pipeFd) ;
    close(epollFd) ;
}

