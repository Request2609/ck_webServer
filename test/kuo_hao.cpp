#include <iostream>
#include <vector>
using namespace std ;
//完成括号匹配
int main() {
    string s ;
    cin >> s ;
    string append="" ;
    int len = s.size() ;
    int index = 0 ;
    int cnt = 0 ;
    //cnt主要作用是控制左右括号书数量保持相等
    while(index < len) {
        if(s[index] == '[') cnt++ ;
        else cnt -- ;
        //当不是左括号的时候，给append添加左括号
        if(cnt<0) {
            cnt++ ;
            append+='[' ;
        }
        index++ ;
    }
    cout << append+s+string(cnt, ']') << endl ;
    return 0;
}

