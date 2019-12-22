#include <iostream>
#include <set>
using namespace std ;

void getLs(int index, string s2, set<string>&ls) {
    string s = s2 ;
    int len = s2.size() ;
    if(index == len) {
        ls.insert(s2) ;
        return ;
    }
    if(s2[index] == '0' || s2[index] == '1') {
        getLs(index+1, s2, ls) ;
    }
    else {
        s[index] = '1' ;
        getLs(index+1, s, ls) ;
        s[index] = '0' ;
        getLs(index+1, s, ls) ;
    }

}

bool getRes(const string& s, const string& s1) {
    int len = s1.size() ;
    int len1 = s.size() ;
    int index = 0 ;
    for(int i=0; i<len; i++) {
        int flag = 0 ;
        int k = i ;
        while(index < len1) {
            if(s[index] != s1[k]) {
                flag = 1 ;
                index= 0 ;
                break ;
            }
            else {
                k++ ;
                index++ ;
            }
        }
        if(flag == 0) return true ;
    }
    return false ;
}

int main() {
    set<string>ls ;
    string  s1 ;
    string s2 ;
    cin >> s1 >> s2 ;
    getLs(0, s2, ls) ;   
    int count = 0;
    for(auto s : ls) {
        int res = getRes(s, s1) ;
        if(res) {
            count ++ ;
        }
    }
    cout << count << endl ;
    return 0;
}

