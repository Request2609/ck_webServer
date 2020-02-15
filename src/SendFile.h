#pragma once
#include <memory>
#include <string.h>
#include <iostream>
#include "Buffer.h"
#include "Channel.h"
#include "Epoll.h"
#include "ReadWrite.h"

using namespace std ;

class channel ;
class sendFile {
public:
    sendFile() {}
    ~sendFile() {}
    int sendChunk(channel* chl) ;
    int sendStaticInfo(channel* chl, const char* buf, unsigned long size) ;
    int newBuffer(Buffer* bf, long pos, string& s) ;
    void setBuf(Buffer* bf, const string& s) ;
    static void setWrite(channel* chl) ;
    static void over(channel* chl) ;
    static string makeChunk(const char* buf) ;
    static void reversestr(char*source,char target[],unsigned int length);
    static void tohex(unsigned long num,char*hexStr);
    static void sendEmptyChunk(int cliFd) ;
};

