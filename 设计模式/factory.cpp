#include <iostream>
#include <memory>
using namespace std  ;
//简单工厂模式

class person {
public :
    person() {
    }
    virtual ~person() {};
    virtual void operation() = 0;
} ;

class employee : public person{
public :
    employee() {}
    ~employee() {}
    void operation() {
        cout << "my ability is do something!" << endl ;
    }
} ;

class boss : public person {
public :
    boss() {
    }
    ~boss() {        
    }
    void operation() {
        cout << "my ability is admin employee!" << endl ;
    }
} ;

class factory {
public :
    static shared_ptr<person> getPerson(int permission) {
        if(permission == 0) {
            //此处不能是实例化一个对象并返回，意思是例如 返回值改成person 这里boss bos; return bos
            //只能以引用的形式返回
            shared_ptr<boss>per =shared_ptr<boss>(new boss);
            return per ;
        }       
        else {
            shared_ptr<employee>per =shared_ptr<employee>(new employee);
            return per ;
        }
    }
} ;

int main() {
    cout << "创建不同角色...." << endl ;
    shared_ptr<person> bos = factory :: getPerson(0) ;
    bos->operation() ;
    shared_ptr<person> emp = factory :: getPerson(1) ;
    emp->operation() ;
    return 0;
}

