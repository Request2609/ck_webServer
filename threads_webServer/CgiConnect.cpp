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
    return 1 ;
}

string cgiConnect :: recvMsg() {
    string ss ;
    while(1) {
        if(recv(sockFd, buf, sizeof(buf), 0)<0) {       
            cout << __LINE__ << "     " << __FILE__ << endl ;
            return "" ;
        } 
        if(buf[0] == '0') {
            break ;      
        }
        ss += buf ;
    }
    return ss ;
}
