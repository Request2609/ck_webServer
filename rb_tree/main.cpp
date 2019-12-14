#include <iostream>
#include "rb_tree.h"

int main() {

    rb_tree rt ;
    while(1) {
        int a ;
        cin >> a ;
        if(a == -1)break ;
        rt.insert_tree(a) ;
    }
    //rt.print_rb_tree() ;
    return 0;
}

