#include <iostream>
#include <list>
using namespace std ;
int main()
{
    list<int>ls ;
    for(int i=0; i<10; i++) {
        int a ;
        cin >> a ;
        ls.push_back(a) ;
    }
    auto res = ls.begin() ;
    while(res != ls.end()) {
        auto ret = res ;
        res ++ ;
        if(res != ls.end()) {
            cout << *ret << "   " << *res << endl ;
        }
    }
    return 0;
}

