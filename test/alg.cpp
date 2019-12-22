#include <iostream>
#include <vector>
#include <algorithm>
using namespace std ;
bool getRes(int mid, const vector<int>&ls, int index) {
    if(mid < 0 || index<0) return false ;
    if(mid == 0) {
        return true ;
    }
    return getRes(mid-ls[index], ls, index-1) || getRes(mid, ls, index-1);   
}

int getLs(int i, vector<int>&ls) {
    ls.clear() ;
    int tmp = i ;
    int sum = 0 ; 
    while(tmp) {
        int cur = tmp%10 ;
        tmp = tmp/10 ;
        ls.push_back(cur) ;
        sum += cur ;
    }
    sort(ls.begin(), ls.end()) ;
    return sum ;
}
void print(vector<int>ls) {
    int len = ls.size() ;
    for(int i=0; i<len; i++) {
        cout << ls[i] <<" " ;
    }
    cout << endl ;
}
int main()
{
    vector<int>ls ;
    int m, n ;
    cin >> m >>n;
    int flag= 0 ;
    bool res ;
    int count = 0 ;
    for(int i=m; i<=n; i++) {
        if(i < 10) {
            continue ;
        }
        flag = 0 ;
        int sum = getLs(i, ls) ;
        if(sum%2) {
            continue ;
        }
        res = getRes(sum/2, ls, ls.size()) ;
        if(res) {
            cout << i << endl ;
            count++ ;
        }
    }
    cout << count << endl ;
    return 0;
}

