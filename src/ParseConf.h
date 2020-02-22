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


class configure {
public:
    ~configure() {}
    static std::shared_ptr<configure>getConfigure() ;
    std::string getServIP() { 
        return servIp ; 
    }
    int getServPort() { 
        return servPort; 
    }
    int getThreadNum() { 
        return threadNum ; 
    }
    std::string getPhpPath() { 
        return phpPath ; 
    }
    std::string getPhpIp() { 
        return phpIp ;
    }
    int getPhpPort() { 
        return phpPort; 
    }
    std::string getLogFile() { 
        return  logFile ;
    }
    int getObjectNumber() { 
        return objectNum ;
    }
    int getCgiPort() {
        return cgiPort ;
    }
    std::string getCgiIP() {
        return cgiIP ;
    }
private:
    void init() ;
    configure():servIp(""), servPort(-1), 
    threadNum(1), phpPath(""), 
    phpIp(""), phpPort(-1), logFile("")
    ,cgiIP(""), cgiPort(-1){}
    static std::shared_ptr<configure>conf ;
    std::string servIp ;
    int servPort ;
    int threadNum ;
    std::string phpPath ;
    std::string phpIp ;
    int phpPort ;
    std::string logFile ;
    int objectNum ;
    std::string cgiIP ;
    int cgiPort ;
};

