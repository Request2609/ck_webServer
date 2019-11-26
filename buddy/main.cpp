#include "buddy.h"

using namespace std ;


void print() {
    cout << "１．创建新进程" << endl ;
    cout << "2 . 取消进程" << endl ;
    cout << "3 . 添加空闲块 " << endl ;
    cout << "4 . 显示内存使用率" << endl ;
    cout << "5 . 设置空闲块大小(默认初始值为4096字节) " << endl ;
    cout << "0 . 退出" << endl ;
}

int main() { 
    int choice ;
    buddy :: init() ;   
    while(1) {
        print() ;
        cin >> choice ;
        switch(choice) {
            case 1:
                buddy::new_process() ;
                break ;
            case 2 :
                buddy::cancer_process() ;
                break ;
            case 3:
                buddy::resize_mem() ;
                break ;
            case 4:
                buddy::print_mem_used_rate() ;
                break ;
            case 5:
                //在刚开始就得设置好，后面该功能不能使用
                buddy::set_free_block_size() ;
                break ;
            case 0:
                exit(0) ;
                break ;
        }
    }
    return 0;
}



