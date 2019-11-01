#ifndef _BUFFER_H_
#define _BUFFER_H_
#include<iostream>
#include<vector>
#include<string>
#include<assert.h>
#include<errno.h>
#include<string.h>
#include"ReadWrite.h"
class Buffer
{
    //用户缓冲区应该和套接字进行绑定
public:
    Buffer(std::string buf) ;
    Buffer(const char* buf) ;
    Buffer() ;
    ~Buffer() {}
public :
    void moveRead() { readIndex++ ; }
    void append(char i) ;
    void setCanProcess(int flag) { this->canProcess = flag ; }
    int getCanProcess() {return canProcess ;}
    std :: string readBuffer(int start, int end) ;
    int retreiveBuffer(int start, int end) ;
    int retreiveBuffer(int n) ;
    void bufferClear() ;
    //传入目标的描述符，读取数据
    int readBuffer(int fd) ;
    int getReadIndex() { return readIndex ; }
    void setReadIndex(int index) { readIndex = index ;}
    int getWriteIndex() { return writeIndex ; }
    void setPostPos(long pos) { contentLen = pos ; }
    long getPostPos() { return contentLen ; }
    char operator[](int i) { return buffer[i] ;}
    int getSize() { return buffer.size(); }
    void changeBuf(std::vector<char> s) { buffer = move(s); }
private :
    //记录post请求的末尾数据长度
    long contentLen ;
    //判断头之后还有没有数据-1标志没有数据，
    //不是-1的话值就是content_length，
    //在读数据的时候值减去每次读到数据的字节数
    //减到0了以后，就回调消息处理函数
    int hasData ;
    //判断是否接受到了'\r\n\r\n'
    int canProcess = 0 ;
    //开始读的地方
    int readIndex = 0;
    //开始写的地方
    int writeIndex = 0;
    std::vector<char>buffer ;
};
#endif


