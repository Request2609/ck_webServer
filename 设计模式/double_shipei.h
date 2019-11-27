#pragma once

#include <iostream>
#include <memory>
using namespace std ;

//适配器模式：双向适配器
class target ;
class adaptee ;
class adapter ;

//目标类
class target {
public :
    target() {}
    virtual ~target() {}
    virtual void request() = 0;
} ;

//具体目标类
class concrete_target : public target {
public :
    void request() {
        cout << "这里请求一个好的适配者服务!" << endl ; 
    }
} ;

//适配者类
class adaptee {
public :
    adaptee() {}
    virtual ~adaptee() {}
    virtual void specific_request() = 0;
} ;

//具体适配者类
class concrete_adaptee : public adaptee {
public :
    void specific_request() {
        cout << "我就是好的适配者!嘻嘻~~~" << endl ;
    }
} ;

//适配器
class adapter : public target, public adaptee{
    //引用适配器对象
    shared_ptr<adaptee>adt ;
    shared_ptr<target> tar ;
public :
    void set_target(shared_ptr<target>tar) {
        this->tar = tar ;
    }
    void set_adaptee(shared_ptr<adaptee>adte) {
        this->adt = adte ;
    }
    void specific_request() ;
    void request() ;
};

