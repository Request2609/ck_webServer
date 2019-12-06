#include "Process.h"

int process :: postRequest(string& tmp, channel* chl, string& bf) {
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
        if(ret == -1) { 
            //cgi请求的数据
            string res = processCgi() ;
            responseHead(chl, "text/html", res.size(), 200, "OK") ;
            getSendBuffer(chl, res) ;
            sendBuffer(chl) ;
            if(ret < 0) {
                return -1 ;
            }
        } 
        else {
            //传入contentlenth
            string file = changePostHtml(chl->getWriteBuffer()->getPostPos(), bf) ; 
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
        readFile(chl) ;
        flag = 1 ;
        messageSend(tmp, chl) ; 
        return POST ;
    }
    return 1 ;
}

void process :: sendBuffer(channel* chl) {
    char buff[4096] ;
    Buffer* buf = chl->getWriteBuffer() ;
    int len = buf->getSize() ;
    for(int i=0; i<len; i++) {
        buff[i] = (*buf)[i] ;
    }
    int ret = 0 ;
    int sockFd = chl->getFd() ;
    if((ret = send(sockFd, buff, sizeof(buff), 0))< 0) {
        cout << __LINE__ << "      " << __FILE__ << endl ;
        return  ;
    }
    if(ret <= len) {
        chl->getWriteBuffer()->bufferClear() ;
        cout << "发送完成!" << endl ;
    }
    else {
        cout << ret << endl ;
        cout << "没有发送完成!" << endl ;
    }
    chl->getReadBuffer()->bufferClear() ;
}

void process :: getSendBuffer(channel* chl, const string res) {
    Buffer* input = chl->getWriteBuffer() ;
    int size = res.size() ;
    for(int i=0; i<size; i++) {
        input->append(res[i]) ;
    }
}

string process :: processCgi() {
    //将所请求的数据转化成绝对路径
    int ret = cgiConnect::connectCgiServer() ;
    if(ret < 0)
        cout << "连接失败！" << endl ;
    string info("1\r\n") ;
    info += cgiArg ;
    //设置环境变量
    if(ret < 0) {
        cout << __LINE__ << "       " << __FILE__ << endl ;
        return "" ;
    }
    if(ret < 0) {
        cout << __LINE__ << "      " << __FILE__ << endl ;
        return "" ;
    }
    //设置环境 ;变量
    char buf[BUFFERSIZE] ;
    strcpy(buf, info.data()) ;
    ret = cgiConnect ::sendMsg(buf) ;
    if(ret < 0) {
        cout <<"发送cgi请求失败！" << endl ;
        return "" ;
    }
    //等待cgi服务器响应
    string ss = cgiConnect :: recvMsg() ;
    //关闭连接套接字
    cgiConnect::closeFd() ;
    return ss ;
}

string process :: changePostHtml(long len, string& bf) {
    //现根据contentlen找相应提交内容
    string tmp = getSubmit(len, bf) ;
    //将要提交的内容
    if(tmp == "") {
        return "";
    }

    if(paths[0] == '/') {
        paths = paths.c_str()+1 ;
    }
    //fastCgi处理请求
    ::FastCgi fc ;
    fc.setRequestId(1) ;
    fc.startConnect() ;
    fc.sendStartRequestRecord() ;
    long size = tmp.size() ;
    char l[10] ;
    sprintf(l, "%ld", size) ;
    char p[1024] ;
    //构造路径,绝对路径
    sprintf(p, "/home/changke/ck_webServer/www/%s", paths.c_str()) ;
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
    string res = fc.readFromPhp() ;
    int ret = res.find("<html>") ;
    if(ret == -1) {
        return "" ;
    }
    res = res.substr(ret, res.size()) ;
    return res ;
}

string process :: getSubmit(long len, string& bf) {
    string info ;
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

string process :: changeHtml() {
    ::FastCgi fc ;
    string res ;
    fc.setRequestId(1) ;
    fc.startConnect() ;
    fc.sendStartRequestRecord() ;
    char buff[1024] ;
    //这里必须是资源的绝对路径~~~~~~
    sprintf(buff, "/home/changke/summer2019/util/summer2019/test/www/%s", paths.c_str()) ;
    fc.sendParams("SCRIPT_FILENAME", buff) ;
    fc.sendParams("REQUEST_METHOD", "GET") ;
    fc.sendEndRequestRecord() ;
    string a =  fc.readFromPhp() ;
    int index = a.find("<html>") ;
    a = a.substr(index, a.size()) ;
    return a ;
}

int process :: getRequest(channel* chl, string& tmp) {
        int ret =  tmp.find("?") ;
        //带参数的url，搜索功能
        if(ret != -1) { 
            cout <<"这是带参数的get 请求" << endl ;
        }
        //要是php的话，就得转成html然后给服务器发
        //有php-fpm完成
        getVersionPath(tmp) ;
        ret = paths.find("php") ;
        if(ret != -1) {
            if(paths[0] == '/') {
                paths = paths.c_str()+1 ;
            }

            string file = changeHtml() ;
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
            flag = 1  ;
            ret = messageSend(tmp, chl) ;
        }
        return 1 ;
}

int process:: sendCgiResult(channel* chl, string res) {
    Buffer* bf =  chl->getWriteBuffer() ;
    long len = res.size() ;
    responseHead(chl, "text/html", len, OK, "OK") ;
    for(int i=0; i<len; i++) {
        bf->append(res[i]) ;
    }
    sss.sendInfo(chl) ;
    return 1 ;
}

//获取请求头
int process :: requestHeader(channel* chl,  vector<pair<int, shared_ptr<channel>>>& mp) {
    canDel = 0 ;
    Buffer* bf =chl->getReadBuffer() ;
    //解析请求行
    int readIndex = bf->getReadIndex() ;
    int writeIndex = bf->getWriteIndex() ;
    string a = bf->readBuffer(readIndex, writeIndex) ;
    //解析请求头
    int index = 0 ;
    string tmp ;
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
        int fd = chl->getFd() ;
        for(auto s=mp.begin(); s!=mp.end(); s++) {
            if(s->first == fd) {
                mp.erase(s) ;
                break ;
            }
        }
    }
    return 0  ;  
}   
//获取请求的长度
int process :: getContentLength(string a, channel* chl) {
    
    int l = chl->getReadBuffer()->getPostPos()  ;
    int pos = 0;
    long p = a.find("\r\n\r\n") ;
    if(l == -1) {
        pos= a.find("Content-Length:") ;
        //没找到，可能发的数据不够，也可能是请求头错误(少见)
        //设置的read buffer 长度为4096，第一个包里面80%有content-length
        if(pos == -1) {
            cout << __FILE__ << "     " << __LINE__ << endl ;
            return -1 ;
        }
        int flag = 0 ;
        string len ;
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
        
        long ret = paths.find(".CGI") ;
        //正在请求CGI程序处理
        if(ret != -1) {
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
    string info ;
    int ret = getSubmitInfo(info, p+4, l, a, chl) ;
    //post只提交登录功能
    if(ret == 1) {
        //处理post请求
        int s = doPost(info) ;
        if(s == 1) {
            return -5;
        }
    }
    return l ;
}

int process :: doPost(string& info) {
    int index = info.find("name") ;
    string name ;
    for(int i=index+5; info[i] != '&'; i++) {
        name+=info[i] ;
    }

    string password ;
    index = info.find("password") ;
    for(int i=index+9; i< (int)info.size() ; i++) {
        password+=info[i] ;
    }
    //验证post请求
    if(name == "la" && password == "ha") {
        return 0 ;
    }
    else {
         return 1 ;
    }
    return 1 ;
}

int process :: getSubmitInfo(string& info, int pos, int l, string &a, channel* chl) {
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

void  process :: responseHead(channel* chl, string type, long len, int statusCode, string tip) {
     //发送响应头
    char buf[1024] ;
    //构造响应头
    string head ;
    sprintf(buf, "%s %d %s\r\nContent-Type: %s\r\nConnection: Keep-Alive\r\nContent-Length:%ld\r\n\r\n",
            version.c_str(), statusCode, tip.c_str(), type.c_str(),len) ;
    //将信息存入输入缓冲区中    
    int l = strlen(buf) ;
    //构造用户态缓冲区
    Buffer* input = chl->getWriteBuffer() ;
    for(int i=0; i<l; i++) {      
        input->append(buf[i]) ;
    }
}   

//处理get请求，发送响应头和
int process :: messageSend(const string& tmp, channel* chl) {
    int s = 0  ;
    //找出现第一个空格的地方
    if(flag == 0)
        getVersionPath(tmp) ;
    //构造响应头
    //解析路径名
    //如果路径只包含“/”，发送初始化页面
    if(paths == "/") {
        struct stat st ;
        int ret = stat(DEFAULT_PATH, &st) ;
        if(ret < 0) {
            cout << __FILE__ << "      " << __LINE__ << strerror(errno)<< endl ;
            return -1 ;
        }
        //获取文件的大小
        long len = st.st_size ;
        paths = DEFAULT_PATH ;
        string type = getFileType() ;
        responseHead(chl, type, len, 200, "OK") ;

        //将文件信息全部写入读缓冲区
        readFile(DEFAULT_PATH, chl) ;
        //设置发送文件的对象!
        s = sss.sendInfo(chl) ;
        if(ret <= 0) {
            canDel = 1 ;
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
        s = sss.sendInfo(chl) ;
    }
    //请求其他的资源
    else {
        readFile(chl) ;
        s = sss.sendInfo(chl) ;
    }
    if(s <= 0) {
        canDel = 1 ;
    }
    flag = 0 ;
    return 1 ;
}

void process :: readFile(channel* chl) {
    string type = getFileType() ; 
    struct stat st ;
    int ret  = stat(paths.c_str(), &st) ;
    if(ret < 0) {
        sendNotFind(chl) ;
        cout << __FILE__ << "    "<< __LINE__ << endl ;
        return ; 
    }
    long len = st.st_size;
    chl->setLen(len+1) ;
    responseHead(chl, type, len, 200, "OK") ;
    readFile(paths.c_str(), chl) ;
}

string process :: getFileType() {
    int index = paths.find(".") ;
    string type ;
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
    int ret = stat("404.html", &st) ;
    if(ret < 0) {
        cout << __FILE__ << "       " << __LINE__ << endl ;
        return  ;
    }
    int len = st.st_size ;
    responseHead(chl, "text/html", len, 404, "NOT FOUND") ;
    readFile("404.html", chl) ;
}

//读文件
void process :: readFile(const char* file, channel* chl) {
    int fd = open(file, O_RDONLY)  ;
    char p[100] ;
    getcwd(p, sizeof(p))  ;
    if(fd < 0) {
        sendNotFind(chl) ;
        cout << __FILE__ << "    " << __LINE__  << endl ;
        return  ;
    }
    int len ;
    char buf[1024] ;
    Buffer* bf = chl->getWriteBuffer() ;
    //读文件
    int sum = 0 ;
    while(1) {
        len = readn(fd, buf, sizeof(buf)) ;
        if(len < 0) {
            cout << __FILE__ << "     " << __LINE__ << endl ;
            return ;
        }
        if(len == 0) {
            break ;
        }
        for(int i=0; i<len; i++) {
            bf->append(buf[i]) ;
        }
        sum+= len ;
        bzero(buf, sizeof(buf)) ;
    }
    
    chl->setLen(sum+1) ;
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
int process :: getVersionPath(string tmp) {
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

//获取请求体
int process :: requestBody(channel* channel_) {
    
    return 1 ;
}

int process :: getMethod(string& line) {
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
