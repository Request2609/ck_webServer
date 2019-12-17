#include <iostream>
#include <vector>
using namespace std ;

long long c[105][105] ;
const int mod = 1000000007 ;
//小q的歌单
void init() {
    //c[i][j]指从a中拿i首歌，从b中拿j首歌组成歌单长度为k的方法数量
    //比如长度为3的有x个，长度为4有y个，求最后长度为ｋ的
    //k = i*3+j*4
    //只需要遍历(i从0到x)(j从0到y)只有等式成立，就得到一组(i,j)。
    //i是从x中选则i个,所以可能性有c<x, i>c[10,3] = 10*9*8/(1*2*3)‘
    //同样可以求得c[y,j]
    c[0][0] = 1 ;
    for(int i=1; i<=100; i++) {
        c[i][0] = 1 ;
        for(int j=1; j<= 100; j++) {
            c[i][j] = (c[i-1][j]+c[i-1][j-1])%mod ;
        }
    }   
}

int main() {
    int ans = 0;
    int k, a, b, x, y ;
    cin>> k ;
    cin >> a >> x >> b >> y ;
    for(int i=0; i<=x; i++) {
        if(i*a <=k && (k-a*i) %b == 0 && (k-a*i)/b<=y) {
            ans = ans+(c[x][i]*c[y][(k-a*i)/b]%mod)%mod ;
        }
    }
    cout << ans << endl;
}
    
