#include <iostream>
#include <string>
using namespace std ;

int main() {
    string ss ;
    cin >> ss ;
    string tmp ="" ;
    int len = ss.size() ;
    int flag = 0 ;
    for(int i=1; i<len; i++) {
    
        flag = 0 ;
        //获取当前指针指向的子串
        tmp = ss.substr(0, i) ;
        if((int)tmp.size() > len-i) {
            break ;
        }
        int k = i;
        int l = tmp.size() ;
        while(k+l<=len) {
            string t = ss.substr(k, l) ;
            if(t != tmp) {
                flag = 1 ;
                break ;
            }
            k += l ;
        }
        if(flag == 0 && k == len) {
            cout << tmp << endl ;
            return 0 ;
        } 
        
    }
    cout << "false" << endl ;
    return 0;
}

