#include <set>
#include <iostream>
using namespace std ;

int main() {
    set<string>s ;
    string a, b ;
    cin >> a >> b ;
    int ans = 0 ;
    int len = a.size() ;
    for(int i=0; i<len; i++) {
        //
        int j = i+b.size()-1 ;  
        //判断后面的字符串是否会越界
        //要是后面的字符串没有要匹配的串长，就跳出
        if(j >= (int)a.size()) {
            break ;
        }
        //从i开始截取b.size的长度
        string cur = a.substr(i, j-i+1) ;
        //如果已经比较过了，跳过
        if(s.count(cur)) continue ;
        s.insert(cur) ;
        int flag =0 ;
        int size = b.size() ;
        for(int k=0; k<size; k++) {
            if(cur[k] != b[k]) {
                if(b[k] == '?') continue ;
                else {
                    flag = 1 ;
                    break ;
                }
            }
        }
        if(flag == 0) {
            ans++ ;
        }
    }
    cout << ans << endl ;
}
