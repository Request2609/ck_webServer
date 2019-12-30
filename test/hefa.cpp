#include <iostream>
#include <string>
#include <vector>
using namespace std ;

int num ;
void dfs(int x, int l, int r, string ss, vector<string>&ls) {
    if(x == num*2) {
        ls.push_back(ss) ;
        return  ;
    }
    if(l < num) {
        dfs(x+1, l+1, r, ss+"(", ls) ;
    }
    if(r < num && l>r) {
        dfs(x+1, l, r+1, ss+")", ls) ;
    }
}
int main() {
    string ss ="" ;
    cin >> num ;
    vector<string>ls ;
    dfs(0, 0, 0, ss, ls) ;
    int l = ls.size() ;
    for(int i=0; i<l; i++) {
        cout << ls[i] << "  "  ;
    }
    cout << endl ;
    return 0;
}


