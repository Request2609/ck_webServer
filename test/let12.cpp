#include <iostream>
#include <vector>
#include <algorithm>
using namespace std ;

class Solution {
public:
    vector<vector<int>> result ;
    vector<vector<int>> fourSum(vector<int>& nums, int target) {    
        //按照降序排列
        sort(nums.begin(), nums.end()) ;
        vector<int>tmp ;
        getRes(nums, target, 0, tmp) ;
        return result ;
    }

    void getRes(vector<int>&nums, int target, int index, vector<int>tmp) {
        int len = nums.size() ;
        if(index == len) return ;
        if(target-nums[index] < 0) {
            return ;
        }
        if(target-nums[index] == 0) {
            result.push_back(tmp) ;
            return ;
        }   
        else {
            tmp.push_back(nums[index]) ;
            getRes(nums, target-nums[index], index+1, tmp) ;
        }
    }
};

int main() {
    vector<int>nums{1, 0, -1, 0, -2, 2} ;
    int target = 0 ;
    Solution ss;
    auto res = ss.fourSum(nums, target) ;
    int len = res.size() ;
    for(int i=0; i<len; i++) {
        int l = res[i].size()  ;
        for(int j=0; j<l; j++) {
            cout << res[i][j]  << "  " ;
        }
        cout << endl ;
    }
    return 0;
}

