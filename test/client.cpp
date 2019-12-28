#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <assert.h>
const int num = 500 ;

int main(int argc, char** argv) {
    if(argc < 3) {
        std :: cout << "usage error!" << std::endl ;
        return 1;
    }
    struct sockaddr_in addr ;
    addr.sin_port = htons(atoi(argv[2])) ;
    addr.sin_family = AF_INET ;
    addr.sin_addr.s_addr = inet_addr(argv[1]) ;
    for(int i=0; i<num; i++) {
        int connfd = socket(AF_INET, SOCK_STREAM, 0) ;
        if(connfd < 0) {
            std::cout << "创建套接字失败!" << std::endl ;
            continue ;
        }
        int ret = connect(connfd, (struct sockaddr*)&addr, sizeof(addr)) ;
        if(ret < 0) {
            std::cout << "连接失败!" << "   " << strerror(errno)<< std::endl ;
            continue ;
        }
    }
    std::cout << "停止连接" << std::endl ;
    getchar() ;
    return 0;
}

