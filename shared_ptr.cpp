#include <iostream>
#include <memory>
using namespace std ;

int main() { 
    shared_ptr<int>ls = make_shared<int>(10) ;
    cout << ls.get() << endl ;
    return 0;
}

