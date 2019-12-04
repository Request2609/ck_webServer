#include <iostream>
#include <map>
#include <vector>
using namespace std ;
class Solution {
public:
    int singleNumber(vector<int>& nums) {
if(nums.size() == 0) {
	return 0 ;
	}
        map<int, int> ls ;
        int len = nums.size() ;
        int count = INT32_MAX; 
        int num =-1 ;
        for(int i=0; i<len; i++) {
            if(ls.find(nums[i]) == ls.end()) {
                ls[nums[i]] = 1 ;
            }
            else {
                ls[nums[i]] ++ ;
            }
        }
        for(auto s : ls) {
            if(s.second < count) {
                count = s.second ;
                num = s.first ;
            }
        }
        cout << num << endl ;
        return num ;
    }
};

