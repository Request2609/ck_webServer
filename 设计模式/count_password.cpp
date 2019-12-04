#include "count_password.h"

//旧的加密适配器系统
string CipherAdapter:: doEncrypt(int key, string ps) {
    cae = make_shared<Caesar>() ;
    string keys = cae->doEncrypt(key, ps) ;
    return keys ;
}

string NewCipherAdapter:: doEncrypt(int key, string ps) {
    nc = make_shared<NewCipher>() ;
    string keys = nc->doEncrypt(key, ps) ;
    return keys ;
}

int main() {
    shared_ptr<DataOperator> dd = make_shared<CipherAdapter>() ;
    //为target类设置密码
    dd->setPassword("hello") ;
    //使用原来的适配器
    string res = dd->doEncrypt(123, dd->getPassword()) ;
    cout << "旧的加密结果!\n" << res << endl ;
    
    dd = make_shared<NewCipherAdapter>() ;
    dd->setPassword("hello") ;
    res = dd->doEncrypt(123, dd->getPassword()) ;
    cout << "新的加密结果!" << endl ;
    cout << res << endl ;
    return 0;
}

