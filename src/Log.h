#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "ParseConf.h"
class log {
public:
    void operator<<(std::string s) ;
    ~log() ;
    static std::shared_ptr<log>getLogObject() ;
    int init() ;
    static std::shared_ptr<log> rcd ;
    static void fullCall(const char* filename, std::size_t size) ;
    void record(const char*s) ;   
private:
    log() {}
    static std::string filename ;
};


