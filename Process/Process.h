#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <memory>
#include <functional>
#include <vector>

using namespace std ;

struct process {
    int createEventFd() ;
    //进程id
    pid_t pid ;
    //为进程设置的管道
    int eventFd ;
    //要调用的进程名称
    string name ;
    //给进程传的参数
    vector<string>list ;
} ;

class processPool {
public:
    processPool(int n) ;
    ~processPool() {}
    void run() ;
    void runChild() ;
    void runParent() ;
private:
    //客户端的fd
    int fd ;
    int index ;
    //是否允许终止
    bool stop ;
    //各个进程的信息
    vector<shared_ptr<process>>processDesc ;
};

int process :: createEventFd() {
    int ret = eventfd(0, 0) ;
    if(ret < 0) {
        cout << __LINE__ << "      " << __FILE__ << endl ;
        return -1 ; 
    }   
    eventFd = ret ;
    return ret ; 
}

processPool :: processPool(int n) {
    processDesc.reserve(n) ;
    //为各个进程申请空间
    for(int i=0; i<n; i++) {
        processDesc[i] = make_shared<process>() ;
    }

    stop = false ;
    //创建子进程
    for(int i=0; i<n; i++) {
        processDesc[i]->createEventFd() ;
        processDesc[i]->pid = fork() ;
        if(processDesc[i]->pid < 0) {
            cout << "创建进程失败！" << endl ;
            cout << __FILE__ << "     " << __LINE__ << endl ;
        }
        if(processDesc[i]->pid == 0) {
            index = -1 ;
            break ;
        }
        //父进程的话,管道使用
        if(processDesc[i]->pid > 0) {
            continue ;
        }
    }
}

void processPool :: run() {
    if(index == -1) runParent() ;
    else runChild() ;
}

//运行子进程
void processPool :: runChild() {
       
}

//运行父进程
void processPool :: runParent() {
    
}
