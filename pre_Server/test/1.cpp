#include <iostream>
#include <future>
using namespace std ;

int main() {
    future<bool> ss ;
    promise<bool>pro ;
    ss = pro.get_future() ;
    pro.set_value(true) ;

    ss.get() ;
    
    return 0;
}

