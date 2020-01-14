#include <iostream>
#include "test.h"

void test :: print() {
    std :: cout << a << std :: endl ;
}

void test :: setA(int a){
    this->a = a ;
}
