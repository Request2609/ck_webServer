#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <memory>

using namespace std ;
class observer ;
class concreteObserver1 ;
class concreteObserver2 ;
class concreteSubject ;
class subject {
public :
    vector<shared_ptr<observer>>oblist ;
    subject() {}
    virtual ~subject() {}
    virtual void attach(shared_ptr<observer> obser) {}
    virtual void detach(shared_ptr<observer>obser) {}
    virtual void notify() {}
    virtual void setState(int state) {}
} ;

class concreteSubject : public subject {
public :
    concreteSubject() {}
    ~concreteSubject() {
    }
    void attach(shared_ptr<observer> obser) ;

    void detach(shared_ptr<observer>obser) {
        for(auto s=oblist.begin(); s!=oblist.end(); s++) {
            if(*s == obser) {
                oblist.erase(s) ;
            }
        } 
    }
    void notify() ;

    void setState(int state) {
        if(this->state == state){
            cout << "状态没变，不通知!" <<  endl ;
            return ;
        } ; 
        this->state = state ;
        cout << "状态发生改变了!" << endl ;
        notify() ;
    }
    int getState() {
        return state ;
    }   
private :
    int state ;
} ;

class observer {
public :
    concreteSubject* csub = NULL ;
public :
    observer() {}
    virtual ~observer() {}
    virtual void update() = 0 ;
};

class concreteObserver1 : public observer{
public :
    concreteObserver1() {}
    ~concreteObserver1() {
    }
    void update() ;
} ;

class concreteObserver2 : public observer{
public :
    concreteObserver2() {}
    ~concreteObserver2() {
    }
    void update() ;
};


