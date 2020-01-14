#include <iostream>
#include <memory>
using namespace std ;
int main()
{
    shared_ptr<int>tmp = shared_ptr<int>(new int(1)) ;
    int a = *tmp ;
    cout << a << endl ;
    return 0;
}

