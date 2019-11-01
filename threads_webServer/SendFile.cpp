#include "SendFile.h"

int sendFile :: sendInfo(channel* chl) {
    int ret = 0 ;
    int index= 0 ;
    string s = "" ;
    char buf[4096] ;
    bzero(buf, sizeof(buf)) ;
    int cliFd = chl->getFd() ;
    Buffer* bf = chl->getWriteBuffer() ;
    int len = bf->getSize() ;
    if(len < 4096) {
        for(int i=0; i<len; i++) {
            buf[index] = (*bf)[i] ;
            index ++ ;
        }
        buf[index] = '\0' ;
        ret = writen(cliFd, buf, sizeof(buf)) ;
        //有数据没有被发送
        if(errno == EAGAIN || ret < len) {
            s = buf ;
            //获取剩下的数据
            s.substr(ret) ;
            ///将剩余的数据写入用户的缓冲区中
            bf->bufferClear() ;
            newBuffer(bf, index, s) ;
            setWrite(chl) ;
        }
        if(ret < 0) {
            over(chl) ;
            cout << __LINE__ << "   " << __FILE__ << endl ;
            return -1 ;
        }
        cout << "移除了fd" << cliFd<< endl ;
        over(chl) ;
        return 0 ;
    }
    int sum = 0 ;
    //数据长度大于4096字节
    for(int i=0; i<len; i++) {
        if(index == 4096) {
            buf[index]  = '\0' ;    
            //这里发送数据信息! 
            ret = writen(cliFd, buf, sizeof(buf)) ;
            if(errno == EAGAIN||ret < SIZE){
                 //没发送完成! 
                s = buf ;
                s = s.substr(ret) ;
                //将剩下的数据全部转移到新的缓冲区
                newBuffer(bf, i, s) ;
                setWrite(chl) ;
                return 1 ;
            }
            if(ret < 0) {
                cout << __LINE__ << "    " << __FILE__ << endl ;
                over(chl) ;
            }
            sum+=index ;
            index = 0 ;
            bzero(buf, sizeof(buf)) ;
        } 
        buf[index] = (*bf)[i] ;
        index++ ;
    }
    //将最后一部分发出去
    if(strlen(buf) > 0) {
        int len = strlen(buf) ;
        ret = writen(cliFd, buf, sizeof(buf)) ;
        //如果没有发出去,数据没发送完成
        if(errno == EAGAIN||ret < len) {
            s = buf ;
            s = s.substr(ret) ;
            setBuf(bf, s) ;    
            setWrite(chl) ;
            return 1 ;
        }
        if(ret < 0 ) {
            cout << errno << "      " << strerror(errno) << endl ;
            cout << __FILE__ << "       " << __LINE__ << endl ;
            over(chl) ;
            return -1 ;
        }
        sum+=len ;
    }
    over(chl) ;
    return 0 ;
}

void sendFile :: over(channel* chl) {
   chl->getEp()->del(chl->getFd()) ;
   cout << "从epollZhongqule " << endl ;
}

void sendFile :: setWrite(channel* chl) {
    chl->enableWriting() ;
    chl->updateChannel() ;
}

int sendFile::newBuffer(Buffer* bf, long pos,  string& s) {
    vector<char>ls ;
    for(int i=0; i<(int)s.size(); i++) {
        ls.push_back(s[i]) ;
    }
    for(int i=pos; i<bf->getSize(); i++) {
        ls.push_back((*bf)[i]) ;
    }
    bf->changeBuf(ls) ;
    return 1 ;
}

void sendFile::setBuf(Buffer* bf, const string& s) {
    for(int i=0; i<s.size(); i++) {
        bf->append(s[i]) ;
    }
}
