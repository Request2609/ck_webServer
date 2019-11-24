#include <iostream>
#include <thread>
#include <queue>
using namespace std ;

queue<int>ls;

int s = 1 ;
int k = 0 ;
int * p = &s ;

int cas(int *p, int* news, int* old) {
    if(p != old) {
        return false ;
    }
    p = news ;
    return true ;
}

void consumer() {
    while(1) {
        while(!cas(p, &s, &k)) {
            cout << "消费队列" << endl ;
        }
        int a = ls.front() ;
        cout << "队列中减少了事件---->" << a << endl ;
        ls.pop() ;
        break ;
    }
}

void producter() {
    int i= 0 ;
    while(1) {
        while(!cas(p, &k, &s)) {
            cout << "生产队列" << endl ;
        }
        cout << "队列中添加了事件---->" << i << endl ;
        ls.push(i) ;
        i++ ;
        if(i==100)
        break ;
    }
}
int main() {
        
    thread t1(consumer) ;
    thread t2(producter) ;
    t1.join() ;
    t2.join() ;
    return 0;
}

