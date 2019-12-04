#pragma once
#include <iostream>
#include <memory>
using namespace std ;

class RefinedAbstration ;
class Abstraction ;
class Implement ;

//接口
class Implement {
public :
    Implement() {}
    virtual ~Implement() {} ;   
    virtual void operationImpl() =0 ;  
};

//实现接口A
class ConcretImplementorA : public Implement {
public :
    void operationImpl() {
        cout << "具体实现A" << endl ;       
    }
} ;

//实现接口B
class ConcretImplementorB : public Implement {
public :
    void operationImpl() {
        cout << "具体实现B" << endl ;
    }
} ;

//抽象类
class Abstraction {
public :
    Abstraction() {}
    virtual ~Abstraction() {}
    virtual void operation() = 0 ;
    void getConcretImp(int flag) ;
protected :
    shared_ptr<Implement> impl  ;
};

//子类
class RefinedAbstration:public Abstraction{
public :
    void operation() {
        cout << "具体类调用具体实现的操作!" << endl ;
        impl->operationImpl() ;
    }   
} ;

