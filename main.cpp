#include <iostream>
#include<thread>
#include "ObjectPool.h"
using namespace std ;
class test {
public :
    void print() {
        cout << "hello world!" << endl ;
    }
private:
    int a ;
} ;


shared_ptr<objectPool<test>> pool ;

void func(int index) {
    for(int i=0; i<1000;i++) {
        auto s = pool->getObject(index) ;
        pool->returnObject(s, index) ;
    }
}

int main() {
    //设置每个对象池中的对象数量
    objectPool<test>::setObjectNumber(10) ;
    pool = objectPool<test>::getPool() ; 
    //获取对象池对象
    //设置队列的数量，并为队列创建对象
    pool->setPoolNum(5) ;
    thread t1(func, 1) ;
    thread t2(func, 2) ;
    thread t3(func, 1) ;
    thread t4(func, 2) ;

    t1.join() ;
    t2.join() ;
    t3.join() ;
    t4.join() ;
 /*   //设置对象池中的池子数量
    //获取第0号对象池中的对象
    auto s1 = pool->getObject(0) ;
    //使用对象
    s1->print() ;
    objectPool<test>::print() ;
    //获取2号对象池中的对象
    auto s2 =  pool->getObject(2) ;
    //归还0号对象池的对象
    pool->returnObject(s1, 0) ;
    //归还2号对象池的对象
    pool->returnObject(s2, 2) ;

    objectPool<test>::print() ;
    //获取0号对象池的对象
    s1 = pool->getObject(0) ;
    //获取3号对象池的对象
    s2 = pool->getObject(3) ;
    //打印各对象池中的对象数量
    objectPool<test>::print() ;
    */
    return 0;
}

