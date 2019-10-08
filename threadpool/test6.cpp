#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <utility>

using namespace std ;

int main() {
    packaged_task<int(int)> foo ;

    //使用lamda表达式初始化一个package_task对象
    packaged_task<int(int)>bar([](int x) { return x+2; }) ;
    foo = move(bar) ;
    //获取共享状态的future
    auto xx = foo.get_future() ;
    thread t1(move(foo), 9) ;
    int value = xx.get() ;
    cout << "数据:" <<  value << endl ;
    t1.join() ;
    return 0;
}

