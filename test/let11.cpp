#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std ;

class Solution {
public:
    vector<string>result ;
    string digit ;
    unordered_map<char, string> ls ;
    vector<string> letterCombinations(string digits) {
        initMap(ls) ;       
        if(digits.empty()) {
            return result ;
        }
        this->digit = digits ;
        queue<string> que ;
        que.push("") ;
        for(auto target : digits) {

            string targetStr = ls[target] ;
            int queueSize = que.size() ;
            for(int i=0; i<queueSize; i++) {
                string tmpStr = que.front() ;
                que.pop() ;
                for(auto tmpChar : targetStr) {
                    que.push(tmpStr+tmpChar) ;
                }
            }
        }
        while(!que.empty()) {
            string ss = que.front() ;
            que.pop() ;
            result.push_back(ss) ;
        }
        return result ;
    }

    //给每个数字建立对应的映射
    void initMap(unordered_map<char, string>& ls) {
        int index = 0 ;
        int k = 3 ;
        for(int i=2; i<=9; i++) {
            pair<char, string> tmp ;
            tmp.first = to_string(i).data()[0] ;
            for(int j=0; j<k; j++) {
                tmp.second += 'a'+index ;
                index++ ;
            }
            ls.insert(tmp) ;
            if(i == 8 || i== 6) k=4 ;
            else k = 3 ;
        }
    }
    void print(const unordered_map<char, string>& ls) {
        for(auto s : ls) {
            cout << s.first << "       " << s.second << endl ;
        }
    }
};

int main() {
    int num ;
    cin >> num ;
    Solution ss ;
    vector<string>cur = ss.letterCombinations(to_string(num)) ;
    int len = cur.size() ;
    cout << len << endl ;
    for(int i=0; i<len; i++) {
        cout << cur[i] << endl ;
    }
    return 0;
}


