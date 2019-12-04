#include <iostream>
#include <vector>
#include <algorithm>
using namespace std ;

class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int prenum = 0 ;
        int precount = INT32_MAX ;
        int curnum = 0 ;
        int curcount = INT32_MAX ;
        int len = nums.size() ;
        curnum = nums[0] ;
        prenum = curnum ;
        curcount =1 ;
        sort(nums.begin(), nums.end()) ;
        for(int i=1; i<len; i++) {
            if(curnum == nums[i]) {
                curcount++ ;
            }
            else {
                if(precount > curcount) {
                    precount = curcount ;
                    prenum = curnum ;
                }
                curcount = 1 ;
                curnum = nums[i] ;
            }
            if(i == len-1) {
                if(precount > curcount) {
                    precount = curcount ;
                    prenum = curnum ;
                }
            }
            cout << nums[i] << "           " << curcount << endl ;
        }   
        cout << prenum<< "     "<< precount << endl ;
        return prenum ;
    }
};
int main() {
    vector<int>ls ;
    for(int i=0; i<10; i++) {
        int a ;
        cin >> a ;
        if(a == 10000) break ;
        ls.push_back(a) ;
    }
    Solution ss ;
    ss.singleNumber(ls) ;
    return 0;
}
