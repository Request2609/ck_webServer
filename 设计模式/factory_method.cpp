#include <iostream>
#include <memory>
using namespace std ;
//工厂方法模式

class tv ;
class changhong_tv ;
class haier_tv ;
class tv_factory ;
class changhong_factory ;

class tv {
public :
    tv() {}
    virtual ~tv() {} ;
    virtual void play()= 0 ;
} ;


class haier_tv : public tv{
public :
    haier_tv() {}
    ~haier_tv() {}
    void play() {
        cout << "我是海尔电视" << endl ; 
    }
} ;

class changhong_tv : public tv {
public :
    changhong_tv() {}
    ~changhong_tv() {}
    void play() {
        cout << "我是长虹电视" << endl ; 
    }
} ;

class tv_factory {
public :
    tv_factory() {}
    virtual ~tv_factory() {}
    virtual shared_ptr<tv> produce() = 0;
} ;

class haier_factory : public tv_factory {
public :
    shared_ptr<tv> produce() {
        cout << "生产海尔电视机" << endl ;
        shared_ptr<haier_tv> haier = shared_ptr<haier_tv>(new haier_tv) ;
        return haier ;
    }
} ;

class changhong_factory : public tv_factory {
public :
    shared_ptr<tv> produce() {
        cout << "生产长虹电视机" << endl ;
        shared_ptr<changhong_tv> ctv = shared_ptr<changhong_tv>(new changhong_tv) ;
        return ctv ;
    }
} ;


int main() {

    shared_ptr<tv_factory>tf = shared_ptr<haier_factory>(new haier_factory)  ;
    shared_ptr<tv> htv = tf->produce() ;
    htv->play() ;
    
    tf = shared_ptr<changhong_factory>(new changhong_factory) ;
    shared_ptr<tv> ctv = tf->produce() ;
    ctv->play() ;
    return 0;
}

