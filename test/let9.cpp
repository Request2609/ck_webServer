#include <iostream>
#include <vector>
#include <string.h>
#include <array>
using namespace std ;

class Solution {
public:
    string convert(string s, int numRows) {
        
        char ls[1000][1000] ;
        memset(ls, '?', sizeof(ls)) ;
        int index = 0 ;
        int len = s.size() ;
        int row = 0 ;
        int col = 0 ;
        if(numRows == 1) {
           return s ;
        }
        while(index < len) {
            while(row < numRows && row >= 0) {
                ls[row][col] = s[index] ;
                row ++ ;
                index++ ;
            }
            row -- ;
            if(row < 0)  break ;
            while(row>0) {
                col++ ;
                row -- ;
                if(row == 0) {
                    break ;
                }
                ls[row][col] = s[index] ;
                index ++ ;
            }
        }
        string tmp ="";
        for(int i=0; i<numRows; i++) {
            for(int j=0; j<col; j++) {
                if (ls[i][j] == '?') {
                    continue ;
                }
                tmp+=ls[i][j] ;
            }
        }
cout << tmp << endl ;
        return tmp ;
    }
};
int main()
{
    string ss ;
    ss = "PAYPALISHIRING";
    Solution st ;
    cout << st.convert(ss, 3) << endl ;
    return 0;
}

