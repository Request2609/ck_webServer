#include <iostream>
#include <vector>
using namespace std ;

class Solution {
public:
    void reverseString(vector<char>& s) {
        int len = s.size() ;
        if(len == 0) {
            return ;
        }
        for(int i=0; i<len/2; i++) {
            int tmp = s[i] ;
            s[i] = s[len-i-1] ;
            s[len-i-1] = tmp ;
        }
    }
};

int main()
{
    vector<char> ls ;
    while(1) {
        char a ;
        cin >> a ;
        if(a == '-') break ;
        ls.push_back(a) ;
    }
    int len = ls.size() ;
    Solution ss ;
    ss.reverseString(ls) ;
    for(int i=0; i<len; i++) {
        cout << ls[i] << endl ;
    }
    return 0;
}

