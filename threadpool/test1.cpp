#include <iostream>
#include <thread>
#include <future>
using namespace std ;

std :: promise<int>prom ;

void print() {
    std :: future<int>fut = prom.get_future() ;
    int x = fut.get() ;
    cout << "value:" << x<< endl ;
}

int main() {
    
    std :: thread t1(print) ;
    prom.set_value(10) ;
    t1.join() ;
    prom = std::promise<int>() ;//prom被move赋值为一个新的对象
    std :: thread th2(print) ;
    prom.set_value(12) ;
    th2.join() ;
    return 0;
}

