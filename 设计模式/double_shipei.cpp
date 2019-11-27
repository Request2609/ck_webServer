#include "double_shipei.h"
void adapter :: request() {
    cout << "具体请求..." << endl ;
    tar->request() ;
}

void adapter::specific_request() {
    adt->specific_request() ;
}

int main() {
    
    shared_ptr<target>tar(new concrete_target) ;
    shared_ptr<adaptee> adt(new concrete_adaptee) ;
    //创建双向适配器
    adapter adter ;
    adter.set_target(tar) ;
    adter.set_adaptee(adt) ;
    //请求适配
    adter.request() ;
    //适配者回应
    adter.specific_request() ;
    return 0;
}

