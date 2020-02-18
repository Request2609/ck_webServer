#include "processPool.h"

void handle(int sig) {
    if(sig == SIGINT) {
        exit(0) ;
    }
}

void init(string& ip, int& port, int& num) {
    int fd = open("../conf/server.json", O_RDONLY);
    if(fd < 0) {
        return ;
    }
    struct stat st ;
    fstat(fd, &st) ;
    char* buf = (char*)mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0) ;
    if(buf == NULL) {
        return  ;
    }
    rapidjson::Document doc ;
    doc.Parse<0>(buf) ;
    rapidjson::Value&cip = doc["cgi ip"] ;
    ip = cip.GetString() ;
    rapidjson::Value&cport = doc["cgi port"] ;
    port = cport.GetInt() ;
    rapidjson::Value&tnum = doc["process number"] ;
    num = tnum.GetInt() ;
    munmap(buf, st.st_size) ;
    close(fd) ;
}   

int main(int argc, char** argv) {
    string ip; 
    int port ;
    int num ;
    init(ip, port, num) ;
    struct sockaddr_in addr ;
    bzero(&addr, sizeof(addr)) ;
    addr.sin_family = AF_INET ;
    int ret = inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) ;
    if(ret < 0) {
        cout << __LINE__ << "        " << __FILE__ << endl ;
        return 0 ;
    }
    addr.sin_port = htons(port) ;
    int listenFd = socket(AF_INET, SOCK_STREAM, 0) ; 
    if(listenFd < 0) {
        cout << __LINE__ << "       " << __FILE__ << endl ;
        return 1;
    }
    int use = 1 ;
    ret = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &use, sizeof(use)) ;
    if(ret < 0) {
        cout << __LINE__ << "       " << __FILE__ << endl ;
        return 0;
    }
    ret = bind(listenFd, (const sockaddr*)&addr, sizeof(addr)) ;
    if(ret < 0) {
        cout << __FILE__ << "       " << __LINE__ << endl ;
        return 0 ;
    }
    ret = listen(listenFd, 100) ;
    if(ret < 0) {
        cout << "监听失败！" << endl ;
        return -1 ;
    }

    //创建
    shared_ptr<processPool<cgiConn>> pool = processPool<cgiConn> :: create(listenFd, num);
    if(pool)
        pool->run() ;
    close(listenFd) ;
    wait(NULL) ;
}


