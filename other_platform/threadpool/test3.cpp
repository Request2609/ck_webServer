#include <iostream>
#include <thread>
#include <future>
#include <unistd.h>
using namespace std ;

void consumer(std :: future<int>&fut) {
    while(1) {
        try{
            int x = fut.get() ;
            cout << "消费:" << x << endl ;
        }catch(exception&e) {
            cout << e.what() << endl ;
        }
    }
}

void produce(promise<int>&prom) {
    for(int i=0; i<10; i++) {
        sleep(1) ;
        try{
            cout << "生产:" << i << endl ;
            prom.set_value(i) ;
        }catch(exception& e) {
            cout << e.what() << endl ;
        }
        if(i == 9) {
            i = 0 ;
        }
    }
}
int main() {
    promise<int>prom ;
    future<int>fut = prom.get_future() ;
    thread th1(consumer, ref(fut)) ;
    thread th2(produce, ref(prom)) ;
    th1.join() ;
    th2.join() ;
    return 0;
}

