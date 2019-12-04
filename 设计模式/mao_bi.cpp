#include "mao_bi.h"

void Pen :: getPenColor(int flag) {
    switch(flag) {
        case 1:
            col = make_shared<Red>() ;
            break ;
        case 2: 
            col = make_shared<Blue>() ;
            break ;
        default : 
            col = make_shared<Black>() ;
            break ;
    }
}   

void SmallPen :: draw() {
    cout << "小笔" << endl ;
    col->bepaint()  ;
}

void BigPen :: draw() {
    cout << "粗笔" << endl ;
    col->bepaint() ;
}

void MiddlePen:: draw() {
    cout << "中性笔" << endl ;
    col->bepaint() ;
}

int main() { 
    shared_ptr<Pen> pen = make_shared<SmallPen>() ;
    pen->getPenColor(1) ;
    pen->draw() ;
    pen->getPenColor(1) ;
    pen->draw() ;
    
    shared_ptr<Pen> pen1 = make_shared<MiddlePen>() ;
    pen1->getPenColor(1) ;
    pen1->draw() ;
    pen1->getPenColor(3) ;
    pen1->draw() ;

    shared_ptr<Pen> pen2 = make_shared<BigPen>() ;
    pen2->getPenColor(2) ;
    pen2->draw() ;
    pen2->getPenColor(1) ;
    pen2->draw() ;
    return 0;
}

