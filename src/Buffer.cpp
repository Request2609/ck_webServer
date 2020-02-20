#include"Buffer.h"

Buffer :: Buffer():canProcess(0) {
    err = log::getLogObject() ;
    contentLen = -1 ;
    readIndex = 0 ;
    writeIndex = 0 ;
    buffer.clear() ; 
}

Buffer :: Buffer(const char* buf) {
    err = log::getLogObject() ;
    contentLen = -1 ;
    assert(buf != NULL) ; 
    int len = strlen(buf) ;
    writeIndex = len ;
    for(int i=0; i<len; i++) {
        buffer.push_back(buf[i]) ;
    }
}

Buffer :: Buffer(std::string buf) {
    err = log::getLogObject() ;
    contentLen = -1 ;
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
    char buffer_[4096] ;
    //接收消息
    int n ;
    if(((n = read(fd, buffer_, sizeof(buffer_))) < 0) && errno != EINTR) {
        std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        return -1 ;
    }
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
    conLen = 0 ;
    conLen =checkBuffer() ;
    //状态机不满足，继续等待
    if(conLen == 0) {
        canProcess = false ;
        return n ;
    }
    //如果收到了最后面的两个"\r\n\r\n",表明可以处理了
    //就可以调用消息处理函数
    if(end == "\r\n\r\n") {
        if(conLen == 1) {
            canProcess = true ;
            return n ;
        }
        else {
            //否则检查请求体
            canProcess = checkBody() ;
            return canProcess ;
        }
    }
    //且指针指向的不是０位置，读指针和写指针在同一位置，清空缓冲区
    if((readIndex != 0 && writeIndex!=0) && readIndex == writeIndex) {
        buffer.clear() ;
        readIndex = 0;
        writeIndex = 0 ;
    }
    return n ;
}

int Buffer::checkBody() {
    int index = buffer.size() ;
    int count =  0 ;
    for(int i=index-1; buffer[i]!='\r'&&buffer[i]!='\n'&&i>0; i--) {
        count++ ;
    }
    if(count == conLen) {
        return true ;
    }
    return false ;
}

bool Buffer::checkBuffer() {
    int index= 0 ;
    std::string s="" ;
    for(int i=0; i<writeIndex; i++) {
        s +=buffer[i]  ;     
    }
    if(s.find("\r\n") != std::string::npos) {
        index = s.find("\r\n") ;
        std::string head = s.substr(0, index) ;
        if(head.find("GET") != std::string::npos) {
            return 1 ;
        }
        if(head.find("POST") != std::string::npos) {
            if(s.find("Content-Length: ") != std::string::npos) {
                index = s.find("Content-Length: ") ;
                head = s.substr(index) ;
                if(head.find("\r\n") == std::string::npos) {
                    return 0 ;
                }
                int end = head.find("\r\n") ;
                head = head.substr(10, end-16) ;
                ///获取长度
                int n = atoi(head.c_str()) ;
                return n ;
            }
            else {
                return 0 ;
            }
        }
    }
    else {
        return 0 ;
    }
    return 1 ;
}      

