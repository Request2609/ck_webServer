#include <iostream>
#include <algorithm>
#include <vector>
using namespace std ;
int main()
{
    int n ;
    cin >> n ;
    vector<int> ls ;
    int ans = 0 ;
    while(n--) {
        int a ;
        cin >>a  ;
        ls.push_back(a) ;
    }
    for(int i=0; i<n; i++) {
        if(n%2 == 0) {
            ans+=ls[i] ;
        }
        else ans-=ls[i] ;
    }
    cout << ans << endl ;
    return 0;
}

