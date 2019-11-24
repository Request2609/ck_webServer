#include <iostream>
#include <future>
#include <thread>
using namespace std ;

int main() {
    packaged_task<int()>task([](){cout << "hello world!"<< endl ; return 666 ;}) ;
    future<int> f1 = task.get_future() ;
    thread t1(ref(task)) ;
    cout << f1.get() << endl ;
    t1.join() ;
}

