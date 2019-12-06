#include <iostream>
#include <fstream>
#include <vector>
using namespace std ;

class Solution {
public:
    int myAtoi(string str) {
        if(str.size() == 0) {
            return 0 ;
        }
        int flag = 0 ;
        int index = 0 ;
        int len = str.size() ;
        //去除空格
        for(int i=0; i<len; i++) {
            if(str[i] == ' ') continue ;
            else {
                if(str[i] == '+') {
                    flag = 2 ;		
                }
                str = str.substr(i) ;
                break ;
            }
        }

        len = str.size() ;
        if(flag == 2 && (str[index] <'0' && str[index] >'9')) {
            return 0 ;
        }
        if(str[index] == '-') {
            flag = 1 ;	
        }
        vector<int>ls ;
        for(int i=(flag==0?0:1); i<len; i++) {
            if(str[i] >= '0' && str[i] <= '9') {
                ls.push_back(str[i]-'0') ;
            }
            else {
                break ;
            }
        }
        long sum = 0 ;
        len = ls.size() ;
        for(int i=0; i<len; i++) {
            sum=ls[i]+sum*10;  
            if(flag == 1 && -1*sum <= INT32_MIN) {
                return INT32_MIN ;        
            }
            else if((flag ==0 || flag == 2) && sum >= INT32_MAX) {
                return INT32_MAX ;
            } 
        }
        if(flag == 1) sum *= -1 ;
        return sum ;
    }
};

int main() {
    string ss ;
    getline(cin, ss) ;
    Solution sol ;
    cout << sol.myAtoi(ss) << endl ;
    return 0;
}

