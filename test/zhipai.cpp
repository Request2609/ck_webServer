#include <iostream>
#include <vector>
#include <algorithm>
using namespace std ;

int main(){
    int num ;
    vector<int>ls ;
    cin >> num ;
    int len = num ; 
    while(num--) {
        int a ;
        cin >> a ;
        ls.push_back(a) ;
    }
    sort(ls.rbegin(), ls.rend()) ;  
    int niu = 0 ;
    int yang = 0 ;
    int k = 0 ;
    while(k<len) {
        niu += ls[k] ;
        if(k+1 > len) break ;
        yang+=ls[k+1] ;
        k += 2 ; 
    }
    cout << niu-yang << endl ;
    return 0;
}

