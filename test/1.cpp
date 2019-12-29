#include <iostream>
#include <future>
#include <thread>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
using namespace std ;

void func() {
    int fd = open("hello", O_RDWR) ;
    if(fd < 0) {
    cout <<"打开文件" << " 错误信息:" <<strerror(errno) << endl ;
    }
    getchar() ;
    close(fd) ;
}
    
int main() {  
    thread t1(func) ;
    thread t2(func) ;
    t1.join() ;
    t2.join() ;
}

