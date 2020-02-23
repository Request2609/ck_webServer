#include"component.h"

int cgiConn :: epollFd ;

std::shared_ptr<log> tool::err = log::getLogObject() ;
int tool::sigOK = 0 ;
int tool :: setNoBlock(int fd) {
    int old = fcntl(fd, F_GETFL) ;
    int ret = fcntl(fd, F_SETFL, old|O_NONBLOCK) ;
    if(ret < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*err) << str ;
        return -1 ;
    }
    return old ;
}

int tool::createEventFd() {
    int fd = eventfd(0, 0) ;
    if(fd < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*err) << str ;
        return -1 ;
    }
    return fd ;
}

int tool ::createSocketPair(int* pipe) {
    if(pipe == NULL) return -1 ;
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipe) ;
    if(ret < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*err) << str ;
        return -1 ; 
    }
    return 1 ;
}


int tool :: addFd(int epollFd, int fd) {
    epoll_event ev ;
    ev.data.fd = fd ;
    ev.events = EPOLLIN|EPOLLET ;
    int ret = epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev) ;
    if(ret < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*err) << str ;
        return -1 ;
    }
    ret = setNoBlock(fd) ;
    if(ret < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*err) << str ;
        return -1 ;
    }
    return 1 ;
}

int tool :: removeFd(int epollFd, int fd) {
    int ret = epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, 0) ;
    if(ret < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*err) << str ;
        return -1 ;
    }
    return 1 ;
}

void cgiConn::process() {
    while(true) {
        bzero(&cd, sizeof(cd)) ;
        int ret = recv(sockFd, &cd, sizeof(cd), 0) ;
        if(ret < 0) {
            if(errno != EAGAIN) {
                tool :: removeFd(epollFd, sockFd) ;
                break ;
            }
            return ;
        }
        else if(ret == 0) {
            int res = tool :: removeFd(epollFd, sockFd) ;
            if(res < 0) {
                return ;
            }
            break ;
        }
        else {
            char buf[BUFSIZ] ;
            getcwd(buf, sizeof(buf)) ;
            if(access("a.CGI", F_OK) == -1) {
                const char* error = "3\r\n404" ;
                if(send(sockFd, error, strlen(error), 0)< 0) {
                    std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
                    (*(tool::err)) << str ;
                }
                tool::removeFd(epollFd, sockFd) ;
                return ;
            }
            cgiProcess(sockFd, cd.path, cd.body) ;
            break ;
        }
    }
}
int cgiConn::cgiProcess(int sockFd, std::string path, std::string arg) {
    pid_t pid ;
    //复制管道描述复位标准输出
    pid = fork() ;
    if(pid == 0) {
        dup2(sockFd, STDOUT_FILENO) ;
        int ret =  execl(path.c_str(), arg.c_str(), NULL) ;
        if(ret < 0) {
            std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
            (*(tool::err)) << str ;
        }
    }
    else {
        tool::removeFd(epollFd, sockFd) ;
        //等待子进程结束
        wait(NULL) ;
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
    return tool::createSocketPair(pipe) ;
}   

