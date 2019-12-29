#include <iostream>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <assert.h>
const int backlog = 5 ;
//一个简单的服务器程序
int main(int argc, char** argv) {
    if(argc < 3) {
        std::cout << "error usage" << std::endl ;
    }
    int connfd ;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t clilen ;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0) ;
    assert(sockfd>=0) ;
    servaddr.sin_family = AF_INET ;
    servaddr.sin_port = htons(atoi(argv[2])) ;
    int flag = 1 ;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]) ;
    int ret = bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) ;
    ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) ;
    std :: cout << strerror(errno) << std:: endl ;
    assert(ret!=-1) ;
    assert(ret!=-1) ;
    ret = listen(sockfd, backlog) ;
    std::cout << "正在侦听" <<std:: endl ;
    assert(ret!=-1) ;
    int num = 0 ;
    while(1) {
        num++ ;
        std::cout << num << std::endl ;
        connfd = accept(sockfd, (struct sockaddr*)&clientaddr, &clilen) ;
        if(connfd < 0) {
            std::cout <<"接受连接失败!" << std::endl ;
            break ;
        }
    }
    getchar() ;
    return 0;
}

