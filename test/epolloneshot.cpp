#include <iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<pthread.h>
#include<iostream>
#define MAX_EVENT_NUMBER 1024//最大事件连接数
#define BUFFER_SIZE 1024//接收缓冲区大小
using namespace std;
struct fds{//文件描述符结构体，用作传递给子线程的参数
    int epollfd;
    int sockfd;
};
int setnonblocking(int fd){//设置文件描述符为非阻塞
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}
void addfd(int epollfd,int fd,bool oneshot){//为文件描述符添加事件
    epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN|EPOLLET;
    if(oneshot){//采用EPOLLONETSHOT事件
        event.events|=EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);
}
void reset_oneshot(int epollfd,int fd){//重置事件
    epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN|EPOLLET|EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}
void* worker(void* arg){//工作者线程(子线程)接收socket上的数据并重置事件
    int sockfd=((fds*)arg)->sockfd;
    int epollfd=((fds*)arg)->epollfd;//事件表描述符从arg参数(结构体fds)得来
    cout<<"start new thread to receive data on fd:"<<sockfd<<endl;
    char buf[BUFFER_SIZE];
    memset(buf,'\0',BUFFER_SIZE);//缓冲区置空
    while(1){
        int ret=recv(sockfd,buf,BUFFER_SIZE-1,0);//接收数据
        if(ret==0){//关闭连接
            close(sockfd);
            cout<<"close "<<sockfd<<endl;
            break;
        }
        else if(ret<0){
            if(errno==EAGAIN){//并非网络出错，而是可以再次注册事件
                reset_oneshot(epollfd,sockfd);
                cout<<"reset epollfd"<<endl;
                break;
            }
        }
        else{
            cout<<buf;
            sleep(5);//采用睡眠是为了在5s内若有新数据到来则该线程继续处理，否则线程退出
        }
    }
    cout<<"thread exit on fd:"<<sockfd;
    //_exit(0);//这个会终止整个进程！！
    return NULL;
}
int main(int argc,char* argv[]){
    if(argc<=2){
        cout<<"argc<=2"<<endl;
        return 1;
    }
    const char* ip=argv[1];
    int port=atoi(argv[2]);
    int ret=0;
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family=AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port=htons(port);
    int listenfd=socket(PF_INET,SOCK_STREAM,0);
    assert(listenfd>=0);
    ret=bind(listenfd,(struct sockaddr*)&address,sizeof(address));
    assert(ret!=-1);
    ret=listen(listenfd,5);
    assert(ret!=-1);
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd=epoll_create(5);
    assert(epollfd!=-1);
    addfd(epollfd,listenfd,false);//不能将监听端口listenfd设置为EPOLLONESHOT否则会丢失客户连接
    while(1){
        int ret=epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);//等待事件发生
        if(ret<0){
            cout<<"epoll error"<<endl;
            break;
        }
        for(int i=0;i<ret;i++){
            int sockfd=events[i].data.fd;
            if(sockfd==listenfd){//监听端口
                struct sockaddr_in client_address;
                socklen_t client_addrlength=sizeof(client_address);
                int connfd=accept(listenfd,(struct sockaddr*)&client_address,&client_addrlength);
                addfd(epollfd,connfd,true);//新的客户连接置为EPOLLONESHOT事件
            }
            else if(events[i].events&EPOLLIN){//客户端有数据发送的事件发生
                pthread_t thread;
                fds fds_for_new_worker;
                fds_for_new_worker.epollfd=epollfd;
                fds_for_new_worker.sockfd=sockfd;
                pthread_create(&thread,NULL,worker,(void*)&fds_for_new_worker);//调用工作者线程处理数据
            }
            else{
                cout<<"something wrong"<<endl;
            }
        }
    }
    close(listenfd);
    return 0;
}
