#include <iostream>
#include <vector>
using namespace std ;

int main() {
    string s ;
    cin >> s ;
    int cur_letter = 0 ;
    double  res = 0;
    //字符种类数
    int count_type = 0 ;
    int index = 0 ;
    int len =s.size() ;
    char tmp;
    if(len==0) {
        return res ;
    }
    tmp = s[index] ;
    cur_letter++ ;
    res = cur_letter ;
    while(1) {
        index ++ ;
        if(index >= len) {
            break ;
        }
        if(tmp != s[index]) {
            
        }
    }
    return 0;
}

