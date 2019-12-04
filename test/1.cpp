#include <iostream>
#include <future>
#include <thread>
using namespace std ;

class A {
public :
    void print() {
        cout << "hello " << endl ;
    }
} ;
void print(shared_ptr<A>a) {
    a->print() ;
}
    
int main() {  
    shared_ptr<A>a = make_shared<A>() ;
    thread t1(print, a) ;
    cout << "world" << endl ;
    t1.join() ;
}

