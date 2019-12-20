#include <iostream>
using namespace std ;
//贪吃的小ｑ
int main() {
    int m, n ;
    int mid, now ;
    int temp ;
    int ok = 0 ;
    cin >> n >> m ;
    int l=1, r=m ;
    while(l != r) {
        ok = 1 ;
        mid = (l+r+1)/2 ;
        //记录巧克力的剩余量
        temp = m ;
        //第一天吃的巧克力数量
        now = mid ;
        //开始吃n天
        for(int j=0; j<n; j++) {
            if(temp < now) {
                ok = 0 ;
                break ;
            }
            //+1表示保证当前天吃的巧克力数量
            //不少于昨天的1/2
            temp -= now ;
            now = (now+1)/2 ;
        }
        //要是按照now当前mid分的话，够吃，将l调大，今儿使第一天吃的量增多
        if(ok) l = mid ;
        //否则，减少第一天的量
        else r = mid-1 ;
    }
    //最会的结果会是l或者r
    cout << l << endl ;
    return 0;
}

