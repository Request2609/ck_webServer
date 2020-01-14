#include "Channel.h"

channel :: channel() {
    events = 0 ;
    input.bufferClear() ;
    output.bufferClear() ;
    cliFd = -1 ;
    len = 0 ;
}

//接收新连接
int channel::handleAccept(int servFd) {

    //设置监听套接字
    sock->setListenFd(servFd) ;
    //获取新客户端连接
    cliFd = sock->acceptSocket() ;
    //设置用户回调
    return cliFd ;
}

void channel :: setFd(int &fd) {
    this->cliFd = fd ;
}

//修改监听事件
int channel :: updateChannel() {
    struct epoll_event ev ;
    ev.data.fd = cliFd ;
    ev.events = events ;
    int ret = epoll_ctl(epFd, EPOLL_CTL_MOD, cliFd, &ev) ;
    if(ret < 0) {
        std::cout << __FILE__ << "      " << __LINE__ << std::endl ;
        return -1 ;
    } 
    return 1 ;
}

int channel :: sendMsg() {
     return 1 ;  
}

int channel :: readMsg() {
    return 1 ;
}

int channel :: handleEvent() {
    if(events&EPOLLIN) { 
        int n = handleRead() ;
        if(n < 0) {
            return -1;
        }
        if(n == 0) {
            return 0 ;
        }
    }
    if(events&EPOLLOUT) {
        //发送数据
        int ret = handleWrite() ;
        if(ret < 1) {
            std::cout << __FILE__ << "     " << __LINE__ << std::endl ;   
            return -1 ;
        }
        //返回０，将这个channel删除
        else {
            return 0 ;
        }
    }
    return 1 ;
}

//执行写回调
int channel :: handleWrite() {
    //写缓冲区
    char buf[SIZE] ;
    int j = 0 ;
    bzero(buf, sizeof(buf)) ;
    int len = output.getWriteIndex() - output.getReadIndex() ;
    //文件长度小于4096
    int sum = 0 ;
    if(len < SIZE) {
        for(int i=0; i< len; i++) {
            buf[j] = output[i] ;
            j++ ;
        }

        buf[j] = '\0' ;
        //写文件长度
        int ret = writen(cliFd, buf, sizeof(buf)) ;
        if(ret < 0) {
            std :: cout << __FILE__ << "     " << std:: endl ;
            return -1 ;
        }
        sum+= ret ;
        //close(cliFd) ;
        input.bufferClear() ;
        //返回１表示可以将连接关闭掉,同事将本channel从EventLoop中删除
        return  1;
    }
    int ret = 0 ;
    //文件长度大于4096
    for(int i=0; i<len; i++) {
        if(j == SIZE) {
            buf[j] = '\0' ;
            ret = writen(cliFd, buf, sizeof(buf)) ;
            if(ret < 0) {
                std::cout << __FILE__ << "     "  << __LINE__<<"      " <<strerror(errno) <<"       num:"<< errno<< std::endl ;  
                return -1 ;
            } 
            sum += ret ;
            bzero(buf, sizeof(buf)) ;
            j = 0 ;
        }
        buf[j] = output[i] ;
        output.moveRead() ;
        j++ ;
    }
    
    if(strlen(buf) > 0) {
        ret = writen(cliFd, buf, sizeof(buf)) ;
        if(ret < 0) {
            std :: cout << __FILE__ << "      " << __LINE__ << std::endl ;
            return -1 ;
        }
        sum+= ret ;
    }
    input.bufferClear() ;
    return 1 ;
}

//执行读回调
int channel :: handleRead() {
    
    //读数据
    int n = input.readBuffer(cliFd) ;
    if(n < 0) {
        return -1 ;
    }
    ///读到0字节数据表明对端已经关闭连接
    if(n == 0) {
        return 0 ;
    }
    //消息设置好后，调用用户回调函数处理    
    if(input.getCanProcess() == true) {  
        readCallBack(this) ;
    }
    return 1 ;
}
