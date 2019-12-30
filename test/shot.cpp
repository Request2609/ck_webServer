#include <iostream>
#include <vector>
using namespace std ;
int main() {
    int x[55], y[55], n ;
    cin >> n ;
    for(int i=0; i<n; i++) {
        cin >> x[i] ;
    }
    for(int j=0; j<n; j++) {
        cin >> x[j] ;
    }
    int maxn = 0 ;
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(i == j) {
                continue ;
            }
            for(int k=0; k<n; k++) {
                if(i == k || j == k) {
                    continue ;
                } 
                if((y[k]-y[j])*(x[i]-x[j]) != (y[i]-y[j])*(x[k]-x[j])) {
                    int nums = 3 ;
                    for(int l=0; l<n; l++) {
                        if(i==l || j == l || k==l) continue ;
                        if((y[i]-y[l])*(x[i]-x[j]) == (y[i]-y[l])*(x[i]-x[l])) nums++ ;
                        else if((y[j]-y[i]) * (y[k]-y[l]) == (x[k]-x[l]) * (x[i]-x[j])) {
                            nums++ ;
                        }
                    }
                    if(nums > maxn) {
                        maxn = nums ;
                    }
                }
            }
        }
    }
    return 0;
}

