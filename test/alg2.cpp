#include <iostream>
using namespace std ;

int getMaxInZi(int x, int y) {
    
    if(x%y== 0) {
        return 1 ;
    }
    int tmp = min(x, y) ;
    tmp /= 2 ;
    for(int i=tmp; i>1; i--) {
        if(x%i == 0 && y%i == 0) {
            return i ;
        }
    }
    return -1 ;
}

int main() {
    int num ;
    cin >> num ;  
    if(num == 1) {
        cout << num << endl ;
    }
    int cur = 1 ;
    int tmp = 1 ;
    for(int i=2; i <= num ;i++) {
        tmp = getMaxInZi(cur, i) ;
        if(tmp == -1) cur *= i ;
        else cur *= tmp ;
    }
    cout << cur%987654321 << endl ;
    return 0;

}

