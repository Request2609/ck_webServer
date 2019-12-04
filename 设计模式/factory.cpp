#include <iostream>
#include <memory>
using namespace std  ;
//简单工厂模式

class fruit {
public :
    fruit() {
    }
    virtual ~fruit() {};
    virtual void operation() = 0;
} ;

class apple : public fruit{
public :
    apple() {}
    ~apple() {}
    void operation() {
        cout << "我是一个苹果！" << endl ;
    }
} ;

class grape : public fruit {
public :
    grape() {
    }
    ~grape() {        
    }
    void operation() {
        cout << "我是一个葡萄！" << endl ;
    }
} ;

class factory {
public :
    static shared_ptr<fruit> getPerson(int permission) {
        if(permission == 0) {
            //此处不能是实例化一个对象并返回，意思是例如 返回值改成person 这里boss bos; return bos
            //只能以引用的形式返回
            shared_ptr<apple>per =shared_ptr<apple>(new apple);
            return per ;
        }       
        else {
            shared_ptr<grape>per =shared_ptr<grape>(new grape);
            return per ;
        }
    }
} ;

int main() {
    cout << "生产水果...." << endl ;
    shared_ptr<fruit> bos = factory :: getPerson(0) ;
    bos->operation() ;
    shared_ptr<fruit> emp = factory :: getPerson(1) ;
    emp->operation() ;
    return 0;
}

