#include <iostream>
#include <map>
#include <vector>

using namespace std ;

class Solution {
public :
    int maxProfit(vector<int>& ls) {
        if(ls.size() <= 1) {
            return 0 ;
        }
        vector<int>diff ;
        int len = ls.size() ;
        for(int i=0; i<len-1; i++) {
            diff.push_back(ls[i+1]-ls[i]) ;
        }
        vector<int>dp(diff.size()) ;
        int profit =max(dp[0], 0) ;
        for(int i=1; i<len; i++) {
            dp[i] = max(dp[i-1], dp[i]) ;
            profit = max(dp[i], profit) ;
        }
        return profit ;
    }
} ;

int main()
{
    vector<int>ls ;
    while(1) {
        int a ;
        cin >> a ;
        if(a == -1) {
            break ;
        }
        ls.push_back(a) ;

    }

}

