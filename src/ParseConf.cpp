#include "ParseConf.h"

std::shared_ptr<configure>configure::conf ;

std::shared_ptr<configure>configure::getConfigure() {
    if(conf == nullptr) {       
        conf = std::shared_ptr<configure>(new configure) ;
        conf->init() ;
    }
    return conf ;
}   


void configure::init() {
    int fd = open("../conf/server.json", O_RDONLY); 
    if(fd < 0) {
           
    }   
    struct stat st ;
    fstat(fd, &st) ;
    char* buf = (char*)mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0) ;
    if(buf == NULL) {

    }
    rapidjson::Document doc ;
    doc.Parse<0>(buf) ;
    rapidjson::Value&ip = doc["ip addr"] ;
    servIp = ip.GetString() ;
    rapidjson::Value&port = doc["port"] ;
    servPort = port.GetInt() ;
    rapidjson::Value&tnum = doc["thread(s) number"] ;
    threadNum = tnum.GetInt() ;
    rapidjson::Value&php_path = doc["FastCGI dir path"] ;
    phpPath = php_path.GetString() ;
    rapidjson::Value&php_ip = doc["php-fpm ip addr"] ;
    phpIp = php_ip.GetString() ;
    rapidjson::Value&php_port = doc["php-fpm port"] ;
    phpPort = php_port.GetInt() ;
    rapidjson::Value&log_file = doc["log file"] ;
    logFile = log_file.GetString() ;
    rapidjson::Value&objNumber = doc["object number"] ;
    objectNum = objNumber.GetInt() ;
    int ret = munmap(buf, st.st_size) ;   
    if(ret < 0) {

    }
    close(fd) ;
}
