#include <iostream>
#include <vector>

using namespace std ;
int main() {

    string ss ;
    cin >> ss ;
    double aver = 0 ;
    int len = ss.size() ;
    int index = 0 ;
    if(len ==  0) {
        cout << 0 << endl ;
        return 0 ;
    }
    int num = 0 ;
    char cur = ss[index] ;
    double sum = 0 ;
    aver = 1 ;
    int count = 1 ;
    num = 1 ;
    while(index < len) {
        if(cur != ss[index]) {
            cur = ss[index] ;     
            count++ ;
        }
        index ++ ;
    }

    printf("%.2lf", len*1.0/count) ;
    return 0 ;
}

