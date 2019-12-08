#include <iostream>
#include <algorithm>
#include <vector>
using namespace std ;

int main()
{
    vector<int> ls ;
    for(int i=0; i<3; i++) {
        int num ;
        cin >> num ;
        ls.push_back(num) ;
    } 
    sort(ls.begin(), ls.end()) ;
    int len = ls.size() ;
    for(int i=0; i<len; i++) {
        cout << ls[i] << endl ;
    }
    return 0;
}

