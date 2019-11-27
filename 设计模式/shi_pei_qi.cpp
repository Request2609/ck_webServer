#include "shi_pei_qi.h"    

void adapter::request() {
    shared_ptr<adaptee>adt(new adaptee) ;
    if(adt ==nullptr) {
        cout << "还没注册适配器" << endl ;
        return ;
    }
    adt->specific_request() ;
}

int main() {
    //创建一个父类
    shared_ptr<target>tar(new adapter) ;
    tar->request() ;
    return 0;
}

