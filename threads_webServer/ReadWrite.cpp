#include"ReadWrite.h"
#include<iostream>

int readn(int fd, void *buf, int n) {
    int nleft = n; //还需要读取的字节数
    char *bufptr =(char*)buf; //指向read函数当前存放数据的位置
    int  nread = 0 ;
    while(nleft > 0){
        if((nread = read(fd, bufptr, nleft)) < 0){
            if(errno == EWOULDBLOCK|errno==EAGAIN){ //遇到中断
                std::cout <<"中断错误!" << std::endl ;
                continue;
            }
            else if(errno == EINTR)           // 其他错误
                continue ;
            else break ;
        }
        else if(nread == 0){ 
            break;
        }
        nleft -= nread;
        bufptr += nread;
    }
    return (n - nleft);
}

int writen(int fd, void *buf, int n){
    int nleft = n;
    char *bufptr = (char*)buf;
    int nwrite;
    while(nleft > 0){
        if((nwrite = write(fd, bufptr, nleft)) < 0){
            //缓冲区写不了了
            if(errno == EAGAIN ||errno == EWOULDBLOCK)
                //返回已经写入的数据总数
                return n-nleft ;
            //屏蔽其他中断信号
            if(errno == EINTR) {
                continue ;
            }
            else {
                return -1;
            }
        }
        if(nwrite == 0) 
            continue;
        nleft -= nwrite;
        bufptr += nwrite;
    }
    return (n-nleft); //  注意这里必须是 n 因为这里保证了 n 字节都被写入
}
