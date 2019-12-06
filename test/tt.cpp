#include <iostream>
#include <map>
using namespace std ;

int main() {
    map<int, char>ls ;
    ls[1] = 'A' ;
    ls[10] = 'B' ;
    ls[8] = 'C' ;
    ls[4] = 'G' ;
    ls[3] = 'R' ;
    for(auto s=ls.rbegin(); s!=ls.rend(); s++) {
        cout << s->first <<"------->" << s->second << endl ;
    }
    return 0;
}

