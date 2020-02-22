#include "Process.h"

const long M_1=(1024*1024) ;
const long G_2=(2048*M_1) ;
//const long G_2=2 ;
const char* DEFAULT_PATH = "index.html" ;
const int BUFLEN = 65535 ;
const int FASTCGI = 1 ;
const int CGI = 2 ; 

int process :: postRequest(std::string& tmp, channel* chl, 
                           std::string& bf) {
    long ret = 0 ;
    //获取到请求路径和版本号
    getVersionPath(tmp) ;
    //获取尾部长度
    ret = getContentLength(bf, chl) ;
    if(ret == -5) {
        sendNotFind(chl) ;
        return POST ;
    }
    else {
        paths = paths.c_str()+1 ;
        if(isExist() < 0) {
            sendNotFind(chl) ;
            return -1 ;
        }
        int ret = paths.find("php") ;
        //如果请求php文件
        //CGI程序
        if(paths.find("php") == std::string::npos) { 
            //cgi请求的数据
            auto res = processCgi() ;
            if(res.find("404") != std::string::npos) {
                sendNotFind(chl) ;
            }
            else {
                responseHead(chl, "text/html", -1, 200, "OK") ;
                sendHeader(chl) ;
                chl->clearBuffer() ;
                getSendBuffer(chl, res) ;
                sss.sendChunk(chl) ;
                if(ret < 0) {
                    return -1 ;
                }
            }
        } 
        else {
            //传入contentlenth
            std::string file = changePostHtml(chl->getWriteBuffer()->getPostPos(), bf) ; 
            if(file == "") {
                sendNotFind(chl) ;
                return -1 ;
            }
            int res= sendCgiResult(chl, file) ;
            if(res < 0) {
                sendNotFind(chl) ;
                return -1 ;
            }
            return 1 ;
        }           
        return POST ;
    }
    return 1 ;
}

void process :: getSendBuffer(channel* chl, const std::string res) {
    Buffer* input = chl->getWriteBuffer() ;
    int size = res.size() ;
    for(int i=0; i<size; i++) {
        input->append(res[i]) ;
    }
}

//发送post请
std::string process :: processCgi() {
    //连接CGI服务器
    int ret = cgiConnect::connectCgiServer() ;
    if(ret < 0) {
        std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        return "";
    }
    struct cgiData cd ;
    bzero(&cd, sizeof(cd)) ;
    cd.isEOF = 1 ;
    strcpy(cd.body, cgiArg.c_str()) ;
    strcpy(cd.path, paths.c_str()) ;
    if(ret < 0) {
        std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        return "" ;
    }
    if(ret < 0) {
        std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        return "" ;
    }
    //向CGI服务器发送请求
    ret = cgiConnect ::sendMsg(cd) ;
    if(ret < 0) {
        std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        return "" ;
    }
    //等待cgi服务器响应
    auto ss = cgiConnect :: recvMsg() ;
    //关闭连接套接字
    cgiConnect::closeFd() ;
    return ss ;
}

std::string process :: changePostHtml(long len, std::string& bf) {
    //现根据contentlen找相应提交内容
    std::string tmp = getSubmit(len, bf) ;
    //将要提交的内容
    if(tmp == "") {
        return "";
    }
    if(paths[0] == '/') {
        paths = paths.c_str()+1 ;
    }
    auto conf = configure::getConfigure() ;
    //fastCgi处理请求
    ::FastCgi fc ;
    fc.setRequestId(1) ;
    fc.startConnect(conf->getPhpIp().c_str(), conf->getPhpPort()) ;
    fc.sendStartRequestRecord() ;
    long size = tmp.size() ;
    char l[10] ;
    sprintf(l, "%ld", size) ;
    char p[1024] ;
    //构造路径,绝对路径
    sprintf(p, "%s%s", conf->getPhpPath().c_str(), paths.c_str()) ;
    fc.sendParams("SCRIPT_FILENAME", p) ;
    //设置方法
    fc.sendParams("REQUEST_METHOD", "POST") ;
    fc.sendParams("CONTENT_LENGTH", l) ;
    fc.sendParams("CONTENT_TYPE", "application/x-www-form-urlencoded") ;
    fc.sendEndRequestRecord() ;
    FCGI_Header h = fc.makeHeader(FCGI_STDIN, 1, size, 0) ;
    fc.sendRequest(h) ;
    fc.sendRequest(const_cast<char*>(tmp.c_str()), size) ;
    FCGI_Header endHead = fc.makeHeader(FCGI_STDIN, 1, 0, 0) ;
    fc.sendRequest(endHead) ;
    std::string res = fc.readFromPhp() ;
    int ret = res.find("<html>") ;
    if(ret == -1) {
        return "" ;
    }
    res = res.substr(ret, res.size()) ;
    return res ;
}

std::string process :: getSubmit(long len, std::string& bf) {
    std::string info ;
    long index = bf.find("\r\n\r\n") ;
    if(index == -1) {
        return "" ;
    }
    index = index+4 ;
    long l = bf.size() ;
    for(int i=index; i<l; i++) {
        info+=bf[i] ;
        len -- ;
        if(len == 0) {
            break ;
        }
    }
    return info ;
}

std::string process :: changeHtml() {
    ::FastCgi fc ;
    std::string res ;
    auto conf = configure::getConfigure() ;
    fc.setRequestId(1) ;
    fc.startConnect(conf->getPhpIp().c_str(), conf->getPhpPort()) ;
    fc.sendStartRequestRecord() ;
    char buff[1024] ;
    //这里必须是资源的绝对路径~~~~~~
    sprintf(buff, "%s%s", conf->getPhpPath().c_str(), paths.c_str()) ;
    fc.sendParams("SCRIPT_FILENAME", buff) ;
    fc.sendParams("REQUEST_METHOD", "GET") ;
    fc.sendEndRequestRecord() ;
    std::string a =  fc.readFromPhp() ;
    int index = a.find("<html>") ;
    a = a.substr(index, a.size()) ;
    return a ;
}

int process :: getRequest(channel* chl, std::string& tmp) {
        int ret =  0 ;
        getVersionPath(tmp) ;
        paths.find("php") ;
        if(paths.find("php") != std::string::npos) {
            if(paths[0] == '/') {
                paths = paths.c_str()+1 ;
            }
            std::string file = changeHtml() ;
            if(isExist() < 0) {
                return -1;
            }
            ///构造响应头
            if(sendCgiResult(chl, file) < 0) {
                sendNotFind(chl) ;
                return -1 ;
            }
            return 1 ;
        }
        else {
            ret = messageSend(tmp, chl) ;
        }
        return 1 ;
}

int process:: sendCgiResult(channel* chl, std::string res) {
    Buffer* bf =  chl->getWriteBuffer() ;
    long len = res.size() ;
    responseHead(chl, "text/html", -1, OK, "OK") ;
    for(int i=0; i<len; i++) {
        bf->append(res[i]) ;
    }
    sendHeader(chl) ;
    chl->clearBuffer() ;
    sss.sendChunk(chl) ;
    return 1 ;
}

void process::processDisConnect(channel* chl,  
                                std::vector<std::pair<int, std::shared_ptr<channel>>>& mp){
    int fd = chl->getFd() ;
    for(auto s=mp.begin(); s!=mp.end(); s++) {
        if(s->first == fd) {
            int id = s->second->getId() ;
            auto tmp = objectPool<channel>::getPool() ;
            //归还对象
            tmp->returnObject(s->second, id) ;
            mp.erase(s) ;
            break ;
        }
    }
}

bool process::isConnect(const std::string& a) {
    //没设置connection选项
    if(a.find("Connection:") == std::string::npos) {
        return true ;
    }
    int index = a.find("Connection:") ;
    std::string s = a.substr(index+11) ;
    int countspace = 0 ;
    for(int i=0; s[i]==' '; i++) {
        countspace ++ ;
    }
    int index1 = s.find("\r\n") ;
    s = s.substr(countspace, index1-countspace) ;
    if(!strcasecmp(s.c_str(), "close")) {
        return false ;
    }
    return true ;
}

//获取请求头
int process :: requestHeader(channel* chl,  
                             std::vector<std::pair<int, std::shared_ptr<channel>>>& mp) {
    err = log::getLogObject() ;
    canDel = 0 ;
    Buffer* bf = chl->getReadBuffer() ;
    //解析请求行
    int readIndex = bf->getReadIndex() ;
    int writeIndex = bf->getWriteIndex() ;
    std::string a = bf->readBuffer(readIndex, writeIndex) ;
    //检查请求头connection字段是close
    if(!isConnect(a)) {
        sss.over(chl) ;
        processDisConnect(chl, mp) ;
        return 1;
    }
    //解析请求头
    int index = 0 ;
    std::string tmp ;
    int e = a.find("\r\n");
    //修改相应的结束标识
    a[e] = '\n' ;
    tmp = a.substr(index, e) ;
    index = e+2;
    int ret = getMethod(tmp) ;
    //如果是GET方法，解析路径名
    if(ret == GET) {
        getRequest(chl, tmp) ;
    }
    //如果是post请求，找出content_length
    if(ret == POST) {
        postRequest(tmp, chl, a) ;
    }
    //当canDel设置成1的时候就会将相应的channel对象移除
    if(canDel == 1) {
        processDisConnect(chl, mp) ;
    }
    return 0  ;  
}   

//获取请求的长度
int process :: getContentLength(std::string a, channel* chl) {
    
    int l = chl->getReadBuffer()->getPostPos()  ;
    int pos = 0;
    long p = a.find("\r\n\r\n") ;
    if(l == -1) {
        pos= a.find("Content-Length:") ;
        //没找到，可能发的数据不够，也可能是请求头错误(少见)
        //设置的read buffer 长度为4096，第一个包里面80%有content-length
        if(pos == -1) {
            std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
            (*err)<<s ;
            return -1 ;
        }
        int flag = 0 ;
        std::string len ;
        for(int i=pos; i<(int)a.length(); i++) {
            if(flag == 1) {
                len+=a[i] ;
            }
            if(a[i] == '\r'||a[i] == '\n') {
                break ;
            }
            if(a[i] == ':')  {
                flag = 1 ;
            } 
        }
        l = atoi(len.c_str()) ;
        
        if(paths.find(".CGI") != std::string::npos) {
            std::cout << "处理CGI" <<std:: endl ;
        //正在请求CGI程序处理
            int r = a.find("\r\n\r\n") ;
            cgiArg = a.substr(r+4, l) ;
            return -4 ;
        }
        chl->getWriteBuffer()->setPostPos(l) ;
    }
    int r = paths.find(".php") ;
    if(r != -1) {
        return 0 ;
    }
    //确定提交的数据
    //根据\r\n\r\n找ontent-length获取信息
    std::string info ;
    getSubmitInfo(info, p+4, l, a, chl) ;
    //post只提交登录功能
    return l ;
}

int process :: getSubmitInfo(std::string& info, int pos, int l, 
                             std::string &a, channel* chl) {
    long len = a.length() ;
    long i = pos ;
    //移动读指针
    while(i<len) {
        //往buf中添加信息
        info += a[i] ;
        i++ ;
        l-- ;
    }
    //用户态修改读取的字节数,每次来post请求读到l为０，表示post接收数据完成
    chl->getReadBuffer()->setPostPos(l) ;
    //如果post请求被读取完成
    //则需要验证信息后给浏览器资源返回资源
    if(l == 0) {
        return 1 ;
    }
    else {
        return -1 ;
    }    
}

void  process :: responseHead(channel* chl, std::string type, 
                              long len, int statusCode, 
                              std::string tip) {
     //发送响应头
    char buf[1024] ;
    //构造响应头
    std::string head ;
    if(len != -1)
        sprintf(buf, "%s %d %s\r\nContent-Type: %s\r\nConnection: close\r\nContent-Length:%ld\r\n\r\n",
            version.c_str(), statusCode, tip.c_str(), type.c_str(),len) ;
    else 
        sprintf(buf, "%s %d %s\r\nContent-Type: %s\r\nConnection: close\r\nTransfer-Encoding: chunked\r\n\r\n",
            version.c_str(), statusCode, tip.c_str(), type.c_str()) ;
    //将信息存入输入缓冲区中    
    int l = strlen(buf) ;
    //构造用户态缓冲区
    Buffer* input = chl->getWriteBuffer() ;
    for(int i=0; i<l; i++) {      
        input->append(buf[i]) ;
    }
}   

int process::sendLittleFile(channel* chl, long len, int fd) {
    int ret ; 
    std::string type = getFileType() ;
    responseHead(chl, type, len, 200, "OK") ;
    ret = sendHeader(chl) ;
    if(ret < 0) {
        std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        canDel = 1 ;
        return -1 ;
    }
    ret = sendfiles(chl, fd, len) ;
    close(fd) ;
    if(ret == 0) {
        sendFile::over(chl) ;
        canDel = 1 ;
        return -1 ;
    }
    else {
        sendFile::setWrite(chl) ;
        canDel = 0 ;
    }
    return 0 ;
}

//处理get请求，发送响应头和
int process :: messageSend(const std::string& tmp, channel* chl) {
    //找出现第一个空格的地方
    if(flag == 0)
        getVersionPath(tmp) ;
    //构造响应头
    //解析路径名
    //如果路径只包含“/”，发送初始化页面
    if(paths == "/") {
        int fd = -1;
        long len = 0 ;
        int ret = getFileInfo(fd, len, DEFAULT_PATH) ;
        if(ret < 0) {
            canDel = 1 ;
            return 1 ;
        }
        std::string type = getFileType() ;
        if(len < G_2) {
            int ret = sendLittleFile(chl, len, fd) ;
            if(ret  <  0) {
                canDel = 1 ;
                return 0;
            }
            close(fd) ;
    
            return  1;
        }
        else {
            responseHead(chl, type, len, 200, "OK") ;
            //先将头发送出去
            sendHeader(chl) ;
            chl->clearBuffer() ;
            readBigFile(chl, fd, len) ;
        }
        return 1 ;
    }
    //将路径前面的/去掉
    if(paths[0] == '/')
        paths = paths.c_str()+1 ;
    //获取资源类型,资源长度，状态码，提示语
    int ret = isExist() ;
    if(!ret) {
        //发送404页面
        sendNotFind(chl) ; 
    }
    //请求其他的资源
    else {
        int fd = -1 ;
        long len = 0 ;
        int ret = getFileInfo(fd, len, paths.c_str()) ;
        if(ret < 0) {
            canDel = 1 ;
            return -1 ;
        }

        if(len>G_2) {
            std::string type = getFileType() ;
            responseHead(chl, type, len, 200, "OK") ;
            sendHeader(chl) ;
            readBigFile(chl, fd, len) ;
        }
        //使用sendfile发送
        else {
            long len = 0 ;
            int ret = getFileInfo(fd, len, paths.c_str()) ;
            if(ret < 0) {
                canDel = 1 ;
                return 1 ;
            }
            std::string type = getFileType() ;
            if(len < G_2) {
                int ret = sendLittleFile(chl, len, fd) ;
                close(fd) ;
                if(ret  < 0) {
                    canDel = 0 ;
                    return 0;
                }
            }
        }
    }
    return 1 ;
}

int process::getFileInfo(int& fd, long& len, const char* paths) {
    fd = open(paths, O_RDONLY) ;
    if(fd < 0) {
        std::string s = std::to_string(__LINE__) +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        return -1;
    }
    struct stat st ;
    fstat(fd, &st) ;
    len = st.st_size ;
    return 1 ;
}   

int process::sendHeader(channel* chl) {
    char buf[BUFSIZ] ;
    bzero(buf, sizeof(buf)) ;
    int cliFd = chl->getFd() ;
    Buffer* bf = chl->getWriteBuffer() ;
    int len = bf->getSize() ;
    int index = 0 ;
    for(int i=0; i<len; i++) {
        buf[index] = (*bf)[i] ;
        index++ ;
    }
    len = strlen(buf) ;
    buf[index] = '\0' ;
    int ret = writen(cliFd, buf, len) ;
    return ret ;
}

int process :: sendfiles(channel* chl, int fd, int len) {
    int cliFd = chl->getFd() ;
    char buf[BUFSIZ] ;
    bzero(buf, sizeof(buf)) ;
    int ret =  0 ;
    ret = sendfile(cliFd, fd, NULL, len) ;
    if(ret < 0) {
        return -1 ;
    }
    int size = len-ret ;
    if(size < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        char* buf = (char*)mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0) ;
        chl->clearBuffer() ;
        int size = strlen(buf) ;
        for(int i=0; i<size; i++) {
            chl->getWriteBuffer()->append(buf[i]) ;
        }
        munmap(buf, len);
        return -1 ;
    }   
    return 0 ;
}

std::string process :: getFileType() {
    int index = paths.find(".") ;
    std::string type ;
    //没找到的话
    if(index == -1) {
        return "text/html" ;
    }
    else {
        int len = paths.length() ;
        type = paths.substr(index+1, len) ;
        if(type == "html"||type == "htm") {
            return "text/html" ;
        }
        if(type == "jpg") {
            return "image/jpg" ;
        }
        if(type == "png") {
            return "image/png" ;
        }
        if(type == "jpeg") {
            return "image/jpeg" ;
        }
        if(type == "pdf") {
            return "application/pdf" ;
        }
        if(type == "gif") {
            return "image/gif" ;
        }
        if(type == "au") {
            return "autio/au" ;
        }
        if(type == "mpeg") {
            return "video/mpeg" ;
        }
        if(type == "mp3") {
            return "audio/mpeg" ;
        }
        if(type == "wav"||type == "mp4") {
            return "audio/wav" ;
        }
        if(type == "ogg") {
            return "application/ogg" ;
        }
        if(type == "css") {
            return "text/css" ;
        }
    }
    return "text/plain;charset=utf-8" ;
}

//资源不可获取
void process :: sendNotFind(channel* chl) {
    struct stat st ;
    chl->getWriteBuffer()->bufferClear() ;
    int fd = open("404.html", O_RDONLY) ;
    if(fd < 0) {
        std::string s = std::to_string(__LINE__) +"  " + __FILE__
            +"    " +strerror(errno)  ;
        (*err)<<s ;
        return ;
    }
    int ret = fstat(fd,  &st) ;
    if(ret < 0) {
        std::string s = std::to_string(__LINE__) +"  " 
            + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        return  ;
    }
    int len = st.st_size ;
    responseHead(chl, "text/html", len, 404, "NOT FOUND") ;
    sendHeader(chl) ;
    chl->clearBuffer() ;
    readBigFile(chl, fd, len) ;
    //分块发送
}

//读文件
void process :: readBigFile(channel* chl, int fd, unsigned long len) {
    if(fd < 0) {
        std::string s = std::to_string(__LINE__) 
            +"  " + __FILE__+"    " +strerror(errno)  ;
        (*err)<<s ;
        canDel = 1 ;
        close(fd) ;
        return  ;
    }
    //读文件
    char * bufp = (char*)mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0) ;
    int ret  = sss.sendStaticInfo(chl, bufp, len) ;
    if(ret == 0) {
        canDel = 1 ;
    }
    else {
        canDel = 0 ;
    }
    munmap(bufp, len) ;
    close(fd) ;
}

//资源是否存在
int process :: isExist() {
    if(access(paths.c_str(), F_OK) != -1) {
        return 1 ;
    }
    else {
        return 0 ;
    }
}

//获取版本号和请求路径
int process :: getVersionPath(std::string tmp) {
    paths.clear() ;
    int pathIndex = tmp.find(' ') ;
    pathIndex += 1 ;
    while(tmp[pathIndex] != ' ') {
        paths += tmp[pathIndex] ;
        pathIndex++ ;
    }
    pathIndex++ ;
    while(pathIndex < (int)tmp.length()) {
        version += tmp[pathIndex] ;
        pathIndex ++ ;
    }
    return 1 ;   
}

int process :: getMethod(std::string& line) {
    if((int)line.find("GET") != -1) {
        method = GET ;
        return GET ;
    }
    else if((int)line.find("POST") != -1) {
        method = POST ;
        return POST ;           
    }
    else {
        method = DEFAULT ;
        return DEFAULT ;
    }
}   
