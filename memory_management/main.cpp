#include "buddy.h"

using namespace std ;


void print() {
    cout << "１．创建新进程" << endl ;
    cout << "2 . 取消进程" << endl ;
    cout << "3 . 添加空闲块 " << endl ;
    cout << "4 . 显示内存使用率" << endl ;
    cout << "5 . 设置空闲块大小 " << endl ;
    cout << "0 . 退出" << endl ;
}

int main() { 
    int choice ;
    buddy :: init() ;   
    print() ;
    while(1) {
        cin >> choice ;
        print() ;
        switch(choice) {
            case 1:
                break ;
            case 2 :
                break ;
            case 3:
                break ;
            case 4:
                break ;
            case 5:
                break ;
            case 0:
                exit(0) ;
                break ;
        }
    }
    return 0;
}


