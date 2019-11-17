#include <iostream>
#include <memory>
using namespace std ;

struct node {
    int i ;
} ;
void func(shared_ptr<struct node>tl) {
    tl = shared_ptr<struct node>(new node) ;
    tl->i = 10 ;
    return ;
}

int main()
{
    shared_ptr<struct node> ptr ;
    func(ptr) ;
    cout << ptr->i << endl ;
    cout << "hello world!" << endl ;
    return 0;
}

