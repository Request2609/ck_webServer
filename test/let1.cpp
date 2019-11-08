#include <iostream>
#include <vector>
#include <algorithm>

//解题思路:先排序，再使用三指针进行求解
//假设三个指针l, m, r, l在最左边，i=k+1,j=nums.size()-1, 移动两个指针求解，
//保证使得nums[i]+nums[j]+nums[k] = ０的解，偏大时r往左移动，偏小时，ｉ往右移动
//当m和r相遇的时候，表示以当前m为最小值的解全部求的,l++进入下一次循环
using namespace std ;
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>>sss ;
        sort(nums.begin(), nums.end()) ;
        int l=0; 
        int r = nums.size()-1 ;
        int m = l+1 ;
        while(nums[l]<=0) {
            vector<int>tmp ;
            while(m < r) {
                if(nums[l] > 0) break ;
                if(l>0&&nums[l] == nums[l-1]) {
                    continue ;
                }
                int res = nums[l]+nums[m]+nums[r] ;
                if(res < 0) {
                    m++ ;
                }
                else if(res > 0) {
                    r-- ;
                }
                else if(res == 0){
                    tmp.push_back(nums[l]) ;
                    tmp.push_back(nums[m]) ;
                    tmp.push_back(nums[r]) ;
                    while(m<r&& nums[m] == nums[m+1]) {
                        m++ ;
                    }
                    while(m<r&&nums[r] == nums[r-1]) {
                        r-- ;
                    }
                    m++ ;
                    r-- ;
                }
            }
            l++ ;
            m = l+1 ;
            sss.push_back(tmp) ;
        }
        return sss ;
    }
};
int main() {
    
    return 0;
}

