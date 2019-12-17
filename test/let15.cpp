#include <iostream>
#include <vector>
using namespace std ;

class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>>res(n, vector<int>(n)) ;
        for(int s= 0, e=n-1, m=1; s<=e; s++, e--) {
            //从左到右横向
            for(int j=s; j<=e; j++) res[s][j] = m++ ;
            for(int i=s+1; i<=e; i++) res[i][e] = m++ ;
            for(int j=e-1; j>= s; j--) res[e][j] = m++ ;
            for(int i=e-1; i>=s+1; i--) res[i][s] = m++ ;
        }
        return res ;
    }   
};

int main() {
    int n ;
    cin >> n ;
    Solution su ;
    vector<vector<int>>res= su.generateMatrix(n) ;
    int row = res.size() ;
    int col = res[0].size() ;
    for(int i=0; i<row; i++) {
        for(int j=0; j<col; j++) {
            cout << res[i][j] <<" " ;
        }
        cout << endl ;
    }
    cout << endl ;
    
    return 0;
}

