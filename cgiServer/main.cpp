#include "processPool.h"

void handle(int sig) {
    if(sig == SIGINT) {
        exit(0) ;
    }
}

int main(int argc, char** argv) {
    if(argc != 3) {
        cout << "error usage" << endl ;
        return 1 ;
    }
    struct sockaddr_in addr ;
    bzero(&addr, sizeof(addr)) ;
    addr.sin_family = AF_INET ;
    int ret = inet_pton(AF_INET, argv[1], &addr.sin_addr) ;
    if(ret < 0) {
        cout << __LINE__ << "        " << __FILE__ << endl ;
        return 0 ;
    }
    addr.sin_port = htons(atoi(argv[2])) ;
    int listenFd = socket(AF_INET, SOCK_STREAM, 0) ; 
    if(listenFd < 0) {
        cout << __LINE__ << "       " << __FILE__ << endl ;
        return 1;
    }
    int use = 1 ;
    ret = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &use, sizeof(use)) ;
    if(ret < 0) {
        cout << __LINE__ << "       " << __FILE__ << endl ;
        return 0;
    }
    ret = bind(listenFd, (const sockaddr*)&addr, sizeof(addr)) ;
    if(ret < 0) {
        cout << __FILE__ << "       " << __LINE__ << endl ;
        return 0 ;
    }
    ret = listen(listenFd, 100) ;
    if(ret < 0) {
        cout << "监听失败！" << endl ;
        return -1 ;
    }
    //中断回收进程后再退出
    signal(SIGINT, handle) ;
    //创建
    shared_ptr<processPool<cgiConn>> pool = processPool<cgiConn> :: create(listenFd, 2);
    if(pool)
        pool->run() ;
    close(listenFd) ;
}


