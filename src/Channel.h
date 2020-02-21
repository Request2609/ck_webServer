#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#include <functional>
#include <sys/epoll.h>
#include <map>
#include <memory>
#include "Epoll.h"
#include "Socket.h"
#include "Buffer.h"
#include "ObjectPool.h"
#include "SendFile.h"
#include "Log.h"
class epOperation ;
class channel ;
class sendFile ;
enum {
    READ = EPOLLIN,
    WRITE = EPOLLOUT,
    ATOMIC= EPOLLONESHOT ,
    ERROR = EPOLLERR,
    ET = EPOLLET 
} ;

//事件分发
class channel {
public :
    typedef std::function<void(channel* chl, 
                               std::vector<std::pair<int, 
                               std::shared_ptr<channel>>>&)> callBack ;
    typedef std::function<void(channel* chl)> wakeCall ;
public :
    channel() ;
    ~channel() {
    }
public :
    bool operator==(channel& chl) ;
    int getFd() {
        return cliFd ;
    }

    void setWakeCall(wakeCall cb) {
        wakeCb = move(cb) ;
    }
    void setFd(int fd) { 
        cliFd = fd ; 
    }   
    void chlCallBack(callBack cb) {
        readCallBack = cb ;
    }
    void setReadCallBack(callBack& cb) {
        readCallBack = cb ;
    }
    void print() ;
    void updateChannel() ; 
    void setWriteCallBack(callBack& cb) {
        writeCallBack = cb ;
    }

    void setCloseCallBack(callBack& cb) {
        closeCallBack = cb ;
    }
    void setTimeoutCallBack(callBack& cb) {
        timeoutCallBack = cb ;
    }
    void  clearBuffer() ;
    void setSock(std::shared_ptr<socketFd>sock) { 
        this->sock = sock; 
    }
    std::shared_ptr<epOperation> getEp() { 
        return ep ; 
    }
    void setEp(std::shared_ptr<epOperation> epo){ 
        ep = epo ;
    }
    void setEpFd(int efd) { 
        epFd = efd ;
    }
    //判断是否收到了一段消息完整的消息"\r\n"结束
    int handleEvent(int fd, std::vector<std::pair<int, std::shared_ptr<channel>>>& tmp, int id) ;
    int handleWrite() ;
    int handleRead(std::vector<std::pair<int, std::shared_ptr<channel>>>&tmp) ;
    int handleAccept(int fd) ;
    //设置channel监听的事件类型
    void setEvents(int event) { 
        events = event ;
    }
    void enableReading() {  
        events|=EPOLLIN ;
    }
    void enableWriting() {  
        events|=EPOLLOUT ;
    }
    void disableRead() { 
        events &= ~EPOLLIN ; 
    }
    void disableWrite() { 
        events &= ~EPOLLOUT ; 
    } 
    int getEvents() { 
        return events ; 
    }
    void setId(int num) { 
        id = num; 
    }
    int getId() { 
        return id; 
    }
    void setLen(long len) { 
        this->len= len ; 
    }
    long getLen() { 
        return len ; 
    }
    Buffer* getReadBuffer() { 
        return  &input ;
    }
    Buffer* getWriteBuffer() { 
        return  &output ;
    }
    static void delFd(int fd, std::map<int, std::shared_ptr<channel>>& tmp) ;
private :
    int id ;
    std ::shared_ptr<epOperation>ep ;
    long len ;
    //管理channel描述符对象的epoll句柄
    int epFd ;
    int flag = 0 ;
    //设置一个标志，是否要继续读
    //感兴趣的事件
    int events ;
    //保存监听套接字信息
    std :: shared_ptr<socketFd> sock ;
    //读回调
    wakeCall wakeCb ;
    callBack readCallBack ;
    //写
    callBack writeCallBack ;
    //关闭回调
    callBack closeCallBack ;
    //超时
    callBack timeoutCallBack ;
    //写完成关闭连接
    callBack writeCompleteCallBack ;
    //写缓冲区
    Buffer input ;
    //度缓冲区
    Buffer output ;
    //要是监听套接字的话，就是监听套接字的文件描述符
    //否则就是目标客户端的fd
    int cliFd ;
    std::shared_ptr<log>err ;
};

#endif
