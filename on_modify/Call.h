#pragma once
#include <unistd.h>
#include "Channel.h"

//唤醒函数，将唤醒套接字缓冲区中的数据读出来,不关闭套接字
void  wakeCb(channel* chl) ;

