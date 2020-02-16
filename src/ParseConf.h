#pragma once
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <memory>
using namespace std ;


class configure {
public:
    ~configure() {}
    static shared_ptr<configure>getConfigure() ;
    string getServIP() { return servIp ; }
    int getServPort() { return servPort; }
    int getThreadNum() { return threadNum ; }
    string getPhpPath() { return phpPath ; }
    string getPhpIp() { return phpIp ;}
    int getPhpPort() { return phpPort; }
    string getLogFile() { return  logFile ;}
    int getObjectNumber() { return objectNum ;}
private:
    void init() ;
    configure():servIp(""), servPort(-1), 
    threadNum(1), phpPath(""), 
    phpIp(""), phpPort(-1), logFile(""){}
    static shared_ptr<configure>conf ;
    string servIp ;
    int servPort ;
    int threadNum ;
    string phpPath ;
    string phpIp ;
    int phpPort ;
    string logFile ;
    int objectNum ;
};

