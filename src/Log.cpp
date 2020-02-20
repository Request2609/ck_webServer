#include "Log.h"
#include "easylog.h"

INITIALIZE_EASYLOGGINGPP
const char* LOG_CONF= "../conf/log.conf" ;

std::string log::filename ;

std::shared_ptr<log>log::rcd ;

log::~log() {
      el::Helpers::uninstallPreRollOutCallback();
}

std::shared_ptr<log>log::getLogObject(){
    if(rcd == nullptr) {
        rcd = std::shared_ptr<log>(new log) ;
        auto res = configure::getConfigure() ;
        filename = res->getLogFile() ;
        rcd->init() ;
    }
    return rcd ;
}

void log::fullCall(const char* file, std::size_t size) {
    rename(file, filename.c_str()) ;
}   

//设置滚动日志
int log::init() {
    el::Configurations conf(LOG_CONF);     
    el::Loggers::reconfigureAllLoggers(conf);
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck) ;
    /// 注册回调函数
    el::Helpers::installPreRollOutCallback(log::fullCall);
    return 1 ;
}

void log::record(const char* s) {
    const std::string ss = s ;
    LOG(INFO) << s ;
}  

void log::operator<<(std::string s) {
    LOG(INFO) << s ;
}
