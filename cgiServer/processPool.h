#pragma once
#include <iostream>
#include <assert.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <map>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <vector>
#include <memory> 
const int EPOLLNUM = 1024 ;
const int BUFFERSIZE = 1024 ;
using namespace std ;

class process ;
class tool ;
class cgiConn ;

class process {
public:
    process() : pid(-1){}
    ~process() {}
public :
    int createSocketPair() ;
    int getReadFd() { return pipe[0] ; }
    int getWriteFd() { return pipe[1] ; }
public :
    pid_t pid ;
private :
    int pipe[2] ;
};

//进程池使用单实例模式
template<typename T>  
class processPool {
public :
    processPool(int listenFd, int processNum = 8) ;
    ~processPool() {}
public :
    //保证时钟只产生一个进程池
    static shared_ptr<processPool<T>> create(int listenFd, 
                                             int processNum = 8) ;
public :
    void run() ;
private :
    //设置进程对应的管道
    void setupSigPipe() ;
    //父进程运行
    void runParent() ;
    //子进程运行
    void runChild() ;
private :
    static const int maxProcessNum = 16 ; //进程池允许的最大子进程数量
    static const int userPreProcess = 65535 ;//每个进程最多能处理的客户数量
    static const int maxEventNum = 10000 ; //epoll最多处理的事件数量
    static shared_ptr<processPool<T>> object ; //进程池的静态实例 

    int curProcessNum ; //进程池目前能处理的进城数量
    int index ; //子进程在进程中的序号
    int epollFd ;  //epoll句柄
    int listenFd ; //监听的文件描述符
    bool stop ; //进程终止标记
    vector<shared_ptr<process>> proDescInfo ; // 保存进程的描述信息
};

class tool {
public :
    static int setNoBlock(int fd) ;
    static int addFd(int epollFd, int fd) ;
    static int removeFd(int epollFd, int fd) ;
    static void addSig(int sig, void(handle)(int), bool restart=true) ;
};

template<typename T> 
shared_ptr<processPool<T>> processPool<T> :: object = nullptr ;

//创建进程池
template<typename T>
processPool<T> :: processPool(int listenFd, int pNum)
    : listenFd(listenFd), curProcessNum(pNum), index(-1), stop(false) {
    assert(pNum > 0 && pNum <= maxProcessNum) ;
    proDescInfo.reserve(pNum)  ;
    for(int i=0; i<pNum; i++) {
        proDescInfo[i] = make_shared<process>() ;
    }
    //创建子进程
    for(int i=0; i<pNum; i++) {
        //创建unix双向匿名管道
        int ret = proDescInfo[i]->createSocketPair() ;
        assert(ret > 0) ;
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
shared_ptr<processPool<T>> processPool<T> :: create(int listenFd, 
                                                    int processNum) {
    if(object == nullptr) {
        object = make_shared<processPool<T>>(listenFd, processNum) ;
    }
    return object ;
}

//父进程在进程中的下标-1
template<typename T>
void processPool<T>::run() {
    if(index != -1) runChild() ;
    else runParent() ;
}

//统一事件
template<typename T> 
void processPool<T> :: setupSigPipe() {
    epollFd = epoll_create(EPOLLNUM) ;
    assert(epollFd > 0) ;
    tool :: addSig(SIGPIPE, SIG_IGN) ;
}

template<typename T> 
void processPool<T> :: runParent() {
    cout << "运行父进程" << endl ; 
    //设置信号
    setupSigPipe() ;
    tool :: addFd(epollFd, listenFd)  ; 
    epoll_event ev[maxEventNum] ;
    int count = 0 ;
    char newConn = 'c' ;
    while(!stop) {
        int number = epoll_wait(epollFd, ev, maxEventNum, -1) ;
        if(number < 0) {
            cout << __LINE__ << "      " << __FILE__ << endl ;
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
                    cout << __LINE__ << "         " << __FILE__ << endl ;
                    return ;
                }
            }
        }   
    }
    close(epollFd) ;
}

//子进程的执行函数
template<typename T>
void processPool<T> :: runChild() {

    setupSigPipe() ;
    cout << "运行子进程" << endl ;
    int pipeFd = proDescInfo[index]->getReadFd() ;
    //将管道描述符加入到epoll中
    tool::addFd(epollFd, pipeFd) ;
    epoll_event ev[maxEventNum]  ;
    //客户事件集合
    map<int, shared_ptr<T>> user ; 
    //为客户端设置处理的对象的池子
    while(!stop) {
        int num = epoll_wait(epollFd, ev, maxEventNum, -1) ;
        if(num < 0&&errno != EINTR) {
            cout << __LINE__ << "        " << __FILE__ << endl ;
            return ;
        } 
        for(int i=0; i<num; i++) {
            int fd =ev[i].data.fd ;
            if(fd == pipeFd&&(ev[i].events&EPOLLIN)) {
                char client ;
                int ret = recv(fd, &client, sizeof(client), 0) ;
                if(ret < 0) {
                    cout << __LINE__ << "      " << __FILE__ << endl ;
                    return ;
                }
                int connFd = accept(listenFd, NULL, NULL) ;
                if(connFd < 0) {
                    cout << __LINE__ << "       " << __FILE__ << endl ;
                    return ;
                }
                cout << "接收新连接" << endl ;
                ret = tool::addFd(epollFd, connFd) ;
                if(ret < 0) {
                    cout << __LINE__ <<  "         " << __FILE__ << endl ;
                    return ;
                }
                //创建用户对象
                user[connFd] = make_shared<T>(epollFd, connFd) ;
            } 
            //处理事件
            else if(ev[i].events&EPOLLIN) {
                cout << "发生可读事件" << endl ;
                user[fd]->process() ;
            }
        }
    }
    close(pipeFd) ;
    close(epollFd) ;
}


class cgiConn {
public :
    cgiConn(int epollFd, int fd) {
        cgiConn::epollFd = epollFd ;
        sockFd = fd ;
        memset(buf, '\0', BUFFERSIZE) ;
        endIndex = 0 ;
    }
    ~cgiConn() {close(sockFd) ;}
    int createSocketPair() {
        int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipe) ;
        if(ret < 0) {
            cout << __LINE__ << "        " << __FILE__ << endl ;
            return -1 ;
        }
        return 1 ;
    }

    void process() {
        while(true) {
            int ret = recv(sockFd, buf, BUFFERSIZE, 0) ;
            if(ret < 0) {
                if(errno != EAGAIN) {
                    tool :: removeFd(epollFd, sockFd) ;
                    break ;
                }
                cout << __LINE__ << "        " << __FILE__ << endl ;
                return ;
            }
            else if(ret == 0) {
                cout << "接收数据" << endl ;
                cout << buf << endl ;
                int res = tool :: removeFd(epollFd, sockFd) ;
                if(res < 0) {
                    return ;
                }
                break ;
            }
            else {
                cout << "接收数据:--->" << buf << endl ;
                cout << "结束" << endl ;
                cgiArg = buf ;
                if(cgiArg[0] == '1') {
                    int a, b ;
                    getArg(a, b) ;
                    cgiProcess(a, b, a+b) ;
                    cout <<"设置完成!" << endl ;
                }
                //处理消息
                //web服务器传来cgi文件路径
                break ;
            }
        }
    }

    int cgiProcess(int a, int b, int t) {
        string body = "<html><meta charset=\"UTF-8\"><head><title>CK_webServerCGI</title></head>";
        body += "<body><p>The result is " + to_string(a) + "+" + to_string(b) + " = " + to_string(t);
        body += "</p></body></html>";
        strcpy(buf, body.data()) ;
        cout <<"发送数据" << buf << endl ;
        int ret  = send(sockFd, buf, sizeof(buf), 0) ;
        cout << "发送完成"<< "     长度:" << ret << endl ;
        if(ret < 0) {
            cout << __LINE__ << "          " <<  __FILE__ << endl ;
            return -1 ;
        }
        bzero(buf, sizeof(buf)) ;
        return 1 ;
    }

    void getArg(int& a, int& b) {
        cout << cgiArg << endl ;
        cgiArg = cgiArg.data()+3 ;
        cout << "数据:" << cgiArg << endl ;
        sscanf(cgiArg.data(), "a=%d&b=%d", &a, &b) ;
    }
public :
    
    //cgi路径
    string cgiPath ;
    //cgi参数
    string cgiArg ;
    static int epollFd ;
    int sockFd ;
    char buf[BUFFERSIZE] ;
    int endIndex ;
    int pipe[2] ;
} ;

int cgiConn :: epollFd ;

int tool :: setNoBlock(int fd) {
    int old = fcntl(fd, F_GETFL) ;
    int ret = fcntl(fd, F_SETFL, old|O_NONBLOCK) ;
    if(ret < 0) {
        cout << __FILE__ << "       " << __LINE__ << endl ;
        return -1 ;
    }
    return old ;
}

int tool :: addFd(int epollFd, int fd) {
    epoll_event ev ;
    ev.data.fd = fd ;
    ev.events = EPOLLIN|EPOLLET ;
    int ret = epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev) ;
    if(ret < 0) {
        cout << __FILE__ << "      " << __LINE__ << endl ;
    }
    ret = setNoBlock(fd) ;
    if(ret < 0) {
        cout << __LINE__ << "      " << __FILE__ << endl ;
        return -1 ;
    }
    return 1 ;
}

int tool :: removeFd(int epollFd, int fd) {
   int ret = epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, 0) ;
   if(ret < 0) {
        cout << __LINE__ << "     " << __FILE__ << endl ;
        return -1 ;
   }
   return 1 ;
}

void tool :: addSig(int sig, void(handle)(int), bool restart) {
    struct sigaction sa ;
    memset(&sa, '\0', sizeof(sa)) ;
    sa.sa_handler = handle ;
    if(restart) 
        int res = sa.sa_flags|SA_RESTART ;
    sigfillset(&sa.sa_mask) ;
}

int process :: createSocketPair() {
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipe) ;
    if(ret < 0) {
        cout << __LINE__ << "      " << __FILE__ << endl ;
        return -1 ; 
    }
    return 1 ;
}   

