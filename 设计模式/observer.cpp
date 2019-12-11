#include "observer.h"
//观察者模式，一般的形式是多个观察者，一个被观察者,当被观察者的行为发生变化的时候
//会有通知机制通知所有的观察者
//类的设计：
//一个抽象被观察者，一个具体的被观察者
//一个观察者接口
//一个集体观察者实现

void concreteSubject::attach(shared_ptr<observer> obser) {
    oblist.push_back(obser) ;
    obser->csub = this ;             
}

void concreteSubject::notify() {
    for(auto s : oblist) {
        s->update() ;
    }   
}

void concreteObserver1::update() {
    cout << "观察者1收到通知!" << endl ;    
    if(csub == NULL) {
        cout << "空对象!" << endl ;
    }   
    else {
        int state = csub->getState() ;
        cout << "获取到对象状态---->" << state << endl ;
    }
} 
void concreteObserver2::update() {
    cout << "观察者2收到通知!" << endl ;    
    if(csub == NULL) {
        cout << "空对象!" << endl ;
    }   
    else {
        int state = csub->getState() ;
        cout << "获取到对象状态---->" << state << endl ;
    }
} 
int main() {
    shared_ptr<subject> sub = make_shared<concreteSubject>() ;
    shared_ptr<observer>obser = make_shared<concreteObserver1>() ;
    shared_ptr<observer> obser1 = make_shared<concreteObserver2>() ;
    sub->attach(obser) ;
    sub->attach(obser1) ;
    sub->setState(1) ;
    return 0;
}

