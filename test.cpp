#include <iostream>
#include <vector>
using  namespace std ;

int main() {
    vector<int>ls ;
    ls.push_back(1) ;
    ls.push_back(9) ;
    ls.clear() ;
    ls.push_back(10) ;
    int num = ls.size() ;
    for(int i=0; i<num; i++) {
        cout << ls[i] << endl ;
    }
    return 0;
}

