#include <iostream>
#include <vector>
using namespace std ;

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        if(matrix.size() == 1) {
            return matrix[0] ;
        }
        vector<int>ls ;
        if(matrix.size() == 0) return ls ;
        int i = 0 ;
        int j = 0 ;
        int n = -1 ;
        int row = matrix.size() ;
        int col = matrix[0].size() ;
        int m = 0;
        int count = (row%10==0?(row/2):(row/2+1)) ;
        int half = ((col%2==0)?col/2:(col/2+1)) ;
        for(int k=0; k < count ; k++) {
            for(i = n+1; i < col-k; i++) {
                ls.push_back(matrix[k][i]) ;
                cout << "k=" << k << "    i=" << i << endl;
               cout << matrix[k][i] << endl ;
            }
            if(half == i) {
                break ;
            }
            for(j = k+1; j < row-k ; j++) {
                ls.push_back(matrix[j][i-1]) ;
                cout << "j=" << j << "i-1" <<i-1 << endl ;
                cout << matrix[j][i-1] << endl ;
            }

            for(m = i-2; m > n; m--) {
                cout << "j-1="<<j-1<<"  m= "<< m << endl ;
                ls.push_back(matrix[j-1][m]) ;
               cout << matrix[j-1][m] << endl ;
            }
            for(n = j-2; n > k; n--) {
                ls.push_back(matrix[n][m+1]) ;
                cout << "n="<<n <<"     m=" << m << endl;
                cout << matrix[n][m+1] << endl ;
            }
        }

        int len = ls.size() ;
        for(int i=0; i<len; i++) {
            cout << ls[i] << endl ;
        }
        return ls ;
    }
};

int main()
{
    Solution ss ;   
    vector<vector<int>>ls ;
    for(int i=0; i<3; i++) {
        vector<int>s ;
        for(int j=0; j<4; j++) {
            int a ;
            cin >>a ;
            s.push_back(a) ;
        }
        ls.push_back(s) ;
    }
    ss.spiralOrder(ls) ;
    return 0;
}

