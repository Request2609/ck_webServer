#include <iostream>
#include <thread>
#include <future>
 using namespace std ;

void print(std :: future<int>&fut) {
    int x = fut.get() ;
    cout << "获取的值：" << x << endl ;
}

int main() {
    //promise对象没有实现operator=语义,进行的赋值操作是move操作
    std :: promise<int>prom ;//生成一个std::promise<int>对象
    //将prom对象和future相关联
    std::future<int>fut = prom.get_future() ;
    //当prom的值没设置好时，线程一直处于阻塞状态
    //将相关的引用座位函数参数
    std :: thread t(print, ref(fut)); 

    int a ;
    cin >> a ;
    //设置相应的int值
    prom.set_value(a) ;
    t.join(
