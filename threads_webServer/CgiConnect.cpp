#include "CgiConnect.h"

int cgiConnect :: sockFd  ;
struct sockaddr_in cgiConnect:: addr  ;
char cgiConnect :: buf[BUFFERSIZE] = {'0'};

cgiConnect :: cgiConnect() {
}

int cgiConnect ::connectCgiServer() {
    sockFd = socket(AF_INET, SOCK_STREAM, 0) ;
    if(sockFd < 0) {
        cout << __LINE__ << "       " << __FILE__ << endl ;
        return -1 ;
    }
    addr.sin_family = AF_INET ;
    addr.sin_port = htons(PORT) ;
    addr.sin_addr.s_addr = inet_addr(IP) ;
    int ret = connect(sockFd, (struct sockaddr*)&addr, sizeof(addr)) ;
    if(ret < 0) {
        cout << __LINE__ << "   " << __FILE__ << endl ;
        return -1 ;
    }
    return sockFd ;
}

int cgiConnect :: sendMsg(const char*  buffer) {
    strcpy(buf, buffer) ;
    if(send(sockFd, buf, sizeof(buf), 0) < 0) {
            cout << __LINE__ << "     " << __FILE__ << endl ;
            return -1 ;
    }
    cout << "发送成功!" << endl ;
    return 1 ;
}

string cgiConnect :: recvMsg() {
    string ss ;
    if(recv(sockFd, buf, sizeof(buf), 0)<0) {       
        cout << __LINE__ << "     " << __FILE__ << "     " << strerror(errno)<< endl ;
        return "" ;
    } 
    cout << "接收到了数据" << endl ;

    ss += buf ;
    return ss ;
}
