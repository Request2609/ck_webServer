#include <iostream>
#include <string>
#include <algorithm>
using namespace std ;

int main() {
    string dst, src ;
    cin >> dst >> src ;
    sort(src.rbegin(), src.rend());   
    int pos = 0 ;
    int k = 0 ;
    int len1 = src.length() ;
    int len = dst.size() ;
    for(int i=k; i<len; i++) {
        if(pos == len1) {
            break ;
        }
        if(dst[i] < src[pos]) {
            dst[i] = src[pos] ;
            pos++ ;
            break ;
        }
    }   
    cout << dst << endl ;
    return 0;
}

