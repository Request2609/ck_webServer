#pragma once

#include <iostream>
#include <memory>
using namespace std ;

class adaptee ;
class target ;
class adapter ;

class adaptee {
public :
    adaptee() {
        cout << "创建了适配器" << endl ;
    }
    ~adaptee() {}
    void specific_request() {
        cout << "我是适配器接口" << endl ;
    }
} ;

class target {
public :
    target() {}
    virtual ~target() {}
    virtual void request() = 0;
} ;

class adapter :public target{
public :
    adapter():adt(nullptr) {}
    ~adapter() {}
    void set_adaptee(shared_ptr<adaptee>adt) { this->adt = adt;}
    void request() ;
private :
    shared_ptr<adaptee> adt ;
};

