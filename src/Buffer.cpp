#include"Buffer.h"

Buffer :: Buffer():canProcess(0) {
    contentLen = -1 ;
    readIndex = 0 ;
    writeIndex = 0 ;
    buffer.clear() ; 
}

Buffer :: Buffer(const char* buf) {
    assert(buf != NULL) ; 
    int len = strlen(buf) ;
    writeIndex = len ;
    for(int i=0; i<len; i++) {
        buffer.push_back(buf[i]) ;
    }
}

Buffer :: Buffer(std::string buf) {
    int len = buf.size() ;
    writeIndex = len ;
    for(int i=0; i<len; i++) {
        buffer.push_back(buf[i]) ;
    }
}

//移动缓冲区中的读指针
int Buffer :: retreiveBuffer(int n) {
    if(readIndex+n >= writeIndex) {
        readIndex = 0 ;
        writeIndex = 0 ;
        buffer.clear() ;
    }  
    else {
        readIndex += n ;
    }
    return readIndex ;
}

//清空用户缓冲区
void Buffer :: bufferClear() {
    readIndex = 0 ;
    writeIndex = 0 ;
    buffer.clear() ;
}

//读取缓冲区，这里注意不会移动readIndex
std :: string Buffer :: readBuffer(int start, int end) {
    std::string a ;
    for(int i=start; i<end; i++) {
        a+=buffer[i] ; 
    }
    return a ;
}

//删除start到end的部分
int Buffer :: retreiveBuffer(int start, int end) {
    //如果读写指针重合，将缓冲区清空
    if(readIndex >= writeIndex) {
        buffer.clear() ;
    }   
    if(start > end) {
        std :: cout << __FILE__ << "    " << __LINE__ << std :: endl ;
        return -1 ;
    }   
    readIndex += (end-start);
    return readIndex ;
}

//想输入缓冲区添加信息
void Buffer :: append(char c) {
    buffer.push_back(c) ;
    writeIndex ++ ;
}

int Buffer :: readBuffer(int fd) {
    char buffer_[1024] ;
    //接收消息
    int n ;
   // std::cout << "开始读------>" << fd << std::endl ;
    if(((n = read(fd, buffer_, sizeof(buffer_))) < 0) && errno != EINTR && errno != 104) {
        std :: cout << __FILE__ << "    " << __LINE__ << "   "<< strerror(errno)<< std :: endl ;
        return -1 ;
    }
    ///std::cout << "读到了"<<n<<"字节" << std::endl ;
    //如果读取到0字节，就关闭连接
    if(n == 0) {
        return 0 ;
    }
    std::string end ;
    //处理消息
    //将消息传到缓冲区中
    for(int i=0; i<n; i++) {
        //写指针自增
        writeIndex ++ ;
        //设置当前channel对象的缓冲区
        buffer.push_back(buffer_[i]) ;
        //表明这段数据可以进行处理了
        if((buffer_[i] == '\r' || buffer_[i] == '\n') && end !="\r\n\r\n") {
            end += buffer_[i]; 
        }
        //判断
        if(end == "\r\n" && buffer_[i+1] != '\r') {
            end.clear() ;
        }
    }
    //如果收到了最后面的两个"\r\n\r\n",表明可以处理了
    //就可以调用消息处理函数
    if(end == "\r\n\r\n") {
        canProcess = true ;
        return n ;
    }
    //且指针指向的不是０位置，读指针和写指针在同一位置，清空缓冲区
    if((readIndex != 0 && writeIndex!=0) && readIndex == writeIndex) {
        buffer.clear() ;
        readIndex = 0;
        writeIndex = 0 ;
    }
    return n ;
}


