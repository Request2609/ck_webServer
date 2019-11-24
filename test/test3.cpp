#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <boost/atomic.hpp>

class A {
    int a ;
public :
    void setA(int aa) {
        a = aa ;
    }
    int getA() {
        return a ;
    }
} ;
int main()
{
    A aa ;
    boost::lockfree:: queue <A> queues ;
    queues.push(aa) ;
    if(!queues.empty()) {

    }
    return 0;
}   

