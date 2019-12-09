#include <iostream>
#include <vector>
using namespace std ;
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string>ls ;
        if(n == 0) {
            return ls ;
        }
        string track="";
        int index = 0 ;
        backtrack(ls, track, n, index) ;
        return ls ;
    }
    void backtrack(vector<string>&ls, string track, int n, int index) {
        if(n == 0 && index == 0) {
            ls.push_back(track) ;
        }
        if(n > 0) backtrack(ls, track+"(", n-1, index+1) ;
        if(index > 0) backtrack(ls, track+")", n, index-1) ;
    }
};

int main() {
    int n ;
    cin >>n ;
    Solution ss ;
    vector<string>ls =ss.generateParenthesis(n) ;
    int len = ls.size() ;
    for(int i=0; i<len; i++) {
        cout << ls[i] << endl;
    }
    return 0;
}

