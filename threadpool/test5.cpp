#include <iostream>
#include <thread>
#include<future> 
using namespace std ;

int print(int x, string a) {
    cout << "数字:" << x << endl ;
    cout << "字符串:" << a << endl ;
    cout << "finish...." << endl ;
    return x ;
}

int main() {

    packaged_task<int(int, string)> task(print) ;//设置package_task函数
    auto ret = task.get_future() ;
    std :: thread th(move(task), 10, "hello") ;
    int value = ret.get() ;
    cout << "函数返回值:" << value << endl ;
    th.join() ;
    return 0;
}
    
