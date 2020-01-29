#pragma once
#include <memory>
#include <string.h>
#include "Buffer.h"
#include "Channel.h"
#include "Epoll.h"
#define SEND_SIZE 4096
using namespace std ;

class sendFile {
public:
    sendFile() {}
    ~sendFile() {}
    int sendInfo(channel* chl) ;
    int newBuffer(Buffer* bf, long pos, string& s) ;
    void setBuf(Buffer* bf, const string& s) ;
    static void setWrite(channel* chl) ;
    static void over(channel* chl) ;
};

