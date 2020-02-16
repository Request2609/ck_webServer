#include <iostream>
#include<signal.h>
#include "TcpServer.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Process.h"
#include "ParseConf.h"

using namespace std ;

void onRead(channel* chl, vector<pair<int, shared_ptr<channel>>>&tmp) {
    //将信息获取完成，再解析
    //解析请求头
    process pro ;
    pro.requestHeader(chl, tmp) ;    
}

void init(eventLoop& loop, 
          tcpServer& serv, connection& conn) {
         serv.create(&loop);
        auto res = configure::getConfigure() ;
        //设置好了ip和端口
        conn.setConf(res->getServIP(), 
                     to_string(res->getServPort())) ;
}

int main(int argc, char** argv) {
    //切换到资源目录
    int ret =  chdir("../www") ;
    signal(SIGPIPE, SIG_IGN) ;
    if(ret < 0) {
        cout << __FILE__ << "      " << __LINE__ << endl ;
        return 0 ;
    }
    eventLoop loop ;
    tcpServer server ;
    connection conn ;
    init(loop, server, conn) ;
    //设置新连接的回调函数
    conn.setReadCallBack(std::bind(onRead, placeholders::_1, placeholders::_2)) ;
    //将新建的连接加入到loop中
    server.addNewConnection(&conn) ;
    //服务器开始工作
    server.start() ;
    //开始等待事件
    loop.loop() ;
    return 0;
}

