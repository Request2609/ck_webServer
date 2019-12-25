#include <iostream>
#include <vector>
using namespace std ;
int main()
{
    int arrnum = 0 ;
    cin >> arrnum ;
    if(arrnum < 0) {
        cout << "NO" << endl ;
        return 0 ;
    }
    while(arrnum--) {
        int cnt1 = 0 ;
        int cnt2 = 0 ;
        int cnt4 = 0 ;
        vector<int>aa ;
        int n ;
        cin >> n ;
        while(n--) {
            int tmp ;
            cin >> tmp ;
            if(tmp%4 == 0) cnt4++ ; 
            else if(tmp%2 == 0) cnt2++ ;
            else cnt1++ ;
        }
        if(cnt2 == 0) {
            if(cnt4 >= cnt1-1) {
                cout << "YES" << endl ;
            }
            else {
                cout << "NO" << endl ;
            }
        }
        else {
            if(cnt4 >= cnt1) {
                cout << "YES" << endl ;
            }
            else {
                cout << "NO" << endl ;
            }
        }
    }
    return 0;
}

