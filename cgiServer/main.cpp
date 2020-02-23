#include "processPool.h"

void init(std::string& ip, int& port, int& num, std::string& cgiPath) {
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
    rapidjson::Value&path = doc["cgi source path"] ;
    cgiPath = path.GetString() ;
    num = tnum.GetInt() ;
    munmap(buf, st.st_size) ;
    close(fd) ;
}   

int main(int argc, char** argv) {
    std::string ip; 
    int port ;
    int num ;
    std::string paths ;
    init(ip, port, num, paths) ;

    //切换到cgi资源目录下
    int ret = chdir(paths.c_str()) ;
    if(ret < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*(tool::err)) << str ;
        return 1;
    }
    struct sockaddr_in addr ;
    bzero(&addr, sizeof(addr)) ;
    addr.sin_family = AF_INET ;
    ret = inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) ;
    if(ret < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*(tool::err)) << str ;
        return 0 ;
    }
    addr.sin_port = htons(port) ;
    int listenFd = socket(AF_INET, SOCK_STREAM, 0) ; 
    if(listenFd < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*(tool::err)) << str ;
        return 1;
    }
    int use = 1 ;
    ret = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &use, sizeof(use)) ;
    if(ret < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*(tool::err)) << str ;
        return 0;
    }
    ret = bind(listenFd, (const sockaddr*)&addr, sizeof(addr)) ;
    if(ret < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*(tool::err)) << str ;
        return 0 ;
    }
    ret = listen(listenFd, 100) ;
    if(ret < 0) {
        std::string str = "  "+ std::to_string(__LINE__)+"   " +__FILE__;
        (*(tool::err)) << str ;
        return -1 ;
    }
    //创建
    std::shared_ptr<processPool<cgiConn>> pool = 
        processPool<cgiConn> :: create(listenFd, num);
    if(pool)
        pool->run() ;
    close(listenFd) ;
    wait(NULL) ;
}


