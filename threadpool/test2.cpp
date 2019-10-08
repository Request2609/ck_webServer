#include <iostream>
#include <thread>
#include <future>
#include <unistd.h>

using namespace std ;

std :: promise<int>prom ;

void consumer() {
    std :: future<int>fut = prom.get_future() ;
    while(1) {
        try{
            int x = fut.get() ;
            cout << "消费:" << x << endl ;
        }catch(exception&e) {
            cout << "异常:"<<e.what() << endl ;
        }
    }
}

void produce() {
    for(int i=0; i<10; i++) {
        sleep(1) ;
        cout << "生产:" << i << endl ;
        prom.set_value(i) ;
        if(i == 9) {
            i = 0 ;
        }
        //prom = std::promise<int>() ;
    }
}

int main() {
    
    thread th1(consumer) ;
    thread th2(produce) ;

    th1.join() ;
    th2.join() ;
    return 0;
}

