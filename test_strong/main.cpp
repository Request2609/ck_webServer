#include <iostream>
#include<signal.h>
#include<functional>
#include "TcpServer.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Process.h"
#include "Create.h"

using namespace std ;
//唤醒函数，将唤醒套接字缓冲区中的数据读出来,不关闭套接字
void wakeCb(channel* chl) {
    int fd = chl->getFd() ;
    int ret  = 0 ; 
    int res= read(fd, &ret, sizeof(ret)) ;
    if(res < 0) {
        cout << __FILE__ << "         " << __LINE__ << endl ;   
        return  ;
    } 
}


void onRead(channel* chl, vector<pair<int, shared_ptr<channel>>>&tmp) {
    //将信息获取完成，再解析
    //解析请求头
    process pro ;
    chl->getReadBuffer()->getCanProcess() ;
    pro.requestHeader(chl, tmp) ;    
}


int main(int argc, char** argv) {
    //切换到资源目录
    int ret =  chdir("../www") ;
    signal(SIGPIPE, SIG_IGN) ;
    if(ret < 0) {
        cout << __FILE__ << "      " << __LINE__ << endl ;
        return 0 ;
    }
    if(argc >3){
        cout << "usage error" << endl ;
        return 0;
    }
    
    eventLoop loop ;
    tcpServer server ;
    connection conn ;
    if(argc == 1) {
        server.create(&loop, "8888") ;
        conn.createSock() ;
        conn.createChannel() ;
    }
    else if(argc == 2) {
        server.create(&loop, argv[1]) ;
        conn.createSock() ;
        conn.createChannel() ;
    }

    if(argc == 3) {
        server.create(&loop);
        //设置好了ip和端
        conn.setConf(argv[1], argv[2]) ;
    }
       
    //设置新连接的回调函数
    conn.setReadCallBack(std::bind(onRead, placeholders::_1, placeholders::_2)) ;
    conn.setWakeCb(std::bind(wakeCb, placeholders::_1)) ;
    //将新建的连接加入到loop中
    server.addNewConnection(&conn) ;
    //服务器开始工作
    server.start() ;
    //开始等待事件
    loop.loop() ;
    return 0;
}

