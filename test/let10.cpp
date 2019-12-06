#include <iostream>
#include <map>
#include <vector>
#include <stack>
using namespace std ;

bool cmp(int a, int b) {
    return a>b ;
}

class Solution {
public:
    string intToRoman(int num) {
        map<int, string>mp ;  
        init(mp) ;
        stack<string>qq ;
        int cur = 0 ;
        string ss ;
        getRes(num, qq, mp, cur) ;
        while(!qq.empty()) {
            ss+=qq.top() ;
            qq.pop() ;
        }
        return ss ;
    }

    void getRes(int tmp, stack<string>&q, map<int, string>&mp, int cur) {
        //和num进行比较,num相比于cur大
        string aa = getCh(mp, tmp, cur) ;     
        int tip = tmp-cur ;
        if(tip == 0) {
            q.push(aa) ;          
        }   
        if(tip > 0 && tip > tmp) {
            q.push(aa) ;
            getRes(tip, q, mp, cur) ;
        }
        //要是当前tmp小于tip
        if(tip > 0 && tip < tmp) {
            getRes(tip, q, mp, cur) ;
            q.push(aa) ;
        }
    }

    string getCh(map<int, string>&mp, int num, int& cur) {
        string aa = "" ;
        for(auto s = mp.rbegin(); s != mp.rend(); s++) {
            if(s->first <= num) {
                cur = s->first ;
                return s->second ;
            }
            aa = s->second ;
        }
        return aa;
    }

    void init(map<int, string>&mp) {
        mp[1] = "I" ;
        mp[5] = "V" ;
        mp[10] = "X" ;
        mp[50] = "L" ;
        mp[100] = "C" ;
        mp[500] = "D" ;
        mp[1000] = "M" ;
        mp[40] = "XL" ;
        mp[90] = "XC" ;
        mp[4] = "IV" ;
        mp[9] = "IX" ;
        mp[400] = "CD" ;
        mp[900] = "CM" ;
    }
};

int main() {
    int num ;
    cin >> num ;
    Solution ss ;
    cout << ss.intToRoman(num) << endl ;
    return 0;
}

