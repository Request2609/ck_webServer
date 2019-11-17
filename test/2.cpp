#include <iostream>
using namespace std ;
struct nnode {
    int a ;
} ;
int main()
{
    struct nnode * tmp = new nnode() ;
    tmp->a= 1 ;
    cout << tmp->a << endl ;
    delete(tmp) ;
    return 0;
}

