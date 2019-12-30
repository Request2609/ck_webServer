#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <string.h>
std::mutex mute ;

void thread_func(int fd) {
    char buf[4096] ;
    bzero(buf, sizeof(buf)) ;
    long a = pthread_self() ;
    sprintf(buf, "这是线程  %ld\n", a) ;
    //std::cout << a << std::endl ;
   // std::lock_guard<std::mutex>lk(mute) ;
    for(int i=0; i<100; i++) {
        write(fd, buf, sizeof(buf)) ;
    }
}


int main() {

    int fd = open("test", O_WRONLY, 0666) ;
    if(fd < 0) {
        std::cout << "打开文件错误!" << std::endl ;
        return 1;
    }
    std::thread t1(thread_func, fd) ;
    std::thread t2(thread_func, fd) ;
    std::thread t3(thread_func, fd) ;
    t1.join() ;
    t2.join() ;
    t3.join() ;
    close(fd) ;
    getchar() ;
    return 0;
}

