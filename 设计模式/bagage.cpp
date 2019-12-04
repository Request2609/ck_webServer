#include "bagage.h"

void Abstraction:: getConcretImp(int flag) {
    if(flag == 1) {
        impl = make_shared<ConcretImplementorA>() ;
    }
    else {
        impl = make_shared<ConcretImplementorB>() ;
    }
}

int main() {

    shared_ptr<Abstraction> abs = make_shared<RefinedAbstration>() ;
    abs->getConcretImp(1) ;
    abs->operation() ;
    abs->getConcretImp(2) ;
    abs->operation() ;
}
