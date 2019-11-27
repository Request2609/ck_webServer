#include <iostream>
#include <memory>
using namespace std ;
//懒汉模式
//单例类的责任过重，在一定程度上违背了单一职责原则
//因为单例类即充当了工厂角色，提供了工厂方法，同时又充当了
//产品角色，包含一些业务方法，将产品的创建和产品的本身功能融合
//在一起
class AA {
public :
    static shared_ptr<AA> get_instance() {
        if(aa == nullptr) {
            aa = shared_ptr<AA>(new AA) ;
        }
        return aa ;
    }
    void print() {
        cout<< "我就是单实例" << endl ; 
    }
    ~AA() {}
private :
    AA() {}
    static shared_ptr<AA> aa ;
} ;
shared_ptr<AA> AA :: aa = nullptr ;

int main()
{
    shared_ptr<AA>aa = AA::get_instance() ;
    aa->print() ;
    return 0;
}

