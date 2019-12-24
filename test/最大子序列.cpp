#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std ;
string s ;

int main() {
    cin >>s ;   
    ostringstream ss ;
    while(!s.empty()) {
        auto it = max_element(s.begin(), s.end()) ;
        ss << *it ;
        s.erase(s.begin(), it+1) ;
    }
    cout << ss.str() << endl ;
    return 0;
}

