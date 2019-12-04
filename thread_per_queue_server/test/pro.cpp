#include <iostream>
#include <future>
#include <vector>

using namespace std ;
vector<promise<int>>pro ;

void print() {
    for(int i=0; i<10; i++) {
        pro[i] = promise<int>() ;
        pro[i].set_value(i) ;
    }
}

void gets() {
    for(int i=0; i<10; i++) {
        future<int>fut = pro[i].get_future() ;
        cout << fut.get() << endl ;
    }
}

int main()
{   pro.reserve(10) ;
    thread t1(print);
    thread t2(gets) ;
    t1.join() ;
    t2.join() ;
    return 0 ;
}

