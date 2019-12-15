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
    rt.print_rb_tree() ;
    cout << "删除树中节点："<< endl ;
    while(1) {
        int a ;
        cout << "请输入要删除的节点(-1退出):" ;
        cin >> a ;
        if(a == -1) break ;
        rt.delete_node(a) ;
        cout << "删除了一个节点！树的节点分布:" << endl ;   
        rt.print_rb_tree() ;
    }       
    return 0;
}

