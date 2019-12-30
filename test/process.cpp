#include <iostream>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <fcntl.h>
using namespace std ;

int main() {
    
    pid_t pid = fork() ;
    if(pid < 0) {
        return 1;
    }
    if(pid == 0) {
        int fd = open("test2", O_WRONLY|O_APPEND) ;

        char buf[4096] ;
        bzero(buf, sizeof(buf)) ;
        sprintf(buf, "这是进程  %d\n", getpid()) ;
        cout << buf << endl ;
        //std::cout << a << std::endl ;
        // std::lock_guard<std::mutex>lk(mute) ;
        for(int i=0; i<100; i++) {
            cout << "写了 " << endl ;
            write(fd, buf, sizeof(buf)) ;
        }
        getchar() ;
        close(fd) ;
    }    
    else {
        int fd = open("test2", O_WRONLY|O_APPEND) ;
        if(fd < 0) {
            cout << strerror(errno) << endl ;
            return 0 ;
        }
        char buf[4096] ;
        bzero(buf, sizeof(buf)) ;
        sprintf(buf, "这是进程  %d\n", getpid()) ;
        cout << buf << endl ;
        //std::cout << a << std::endl ;
        // std::lock_guard<std::mutex>lk(mute) ;
        for(int i=0; i<100; i++) {
            cout << "写数据" << endl ;
            write(fd, buf, sizeof(buf)) ;
        }
        getchar() ;
        close(fd) ;
    }
    wait(NULL) ;
    return 0;
}

