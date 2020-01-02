#include <iostream>
#include <string.h>
using namespace std ;

int main() {
    int a[10001] ;
    memset(a, -1, sizeof(a)) ;
    int maxs = -1 ;
    int n ;
    cin >> n ;
    int ss = 1;
    for(int i=0; i<n; i++) {
        cin >>ss ;
        if(ss > maxs) {
            maxs = ss ;
        }
        a[ss] = 1 ;
    }
    for(int k=0; k<maxs; k++) {
        if(a[k] != 1) {
            cout << k << endl ;
        }
    }
    return 0;
}

