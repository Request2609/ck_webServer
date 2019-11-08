#include <iostream>
#include <vector>
#include <algorithm>

using namespace std ;
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end()) ;
        int len = nums.size() ;
        int l=0, r=0, k=0 ;
        int tt = nums[0]+nums[1]+nums[2] ;

        for(int i=0; i < len-2; i++) {
            l = i ;
            k = l+1 ;
            r = len-1 ;
            int sum = nums[l]+nums[k]+nums[r] ;
            while(k<r) {
                if(abs(tt-target) > abs(sum-target)) {
                    cout << tt << "   " << sum << endl ;
                    tt = nums[l]+nums[r]+nums[k] ;
                }
                int flag = target-sum ;
                if(flag == 0) {
                    return target ;
                }
                else if(flag < 0) {
                    k++ ;
                }
                else {
                    r-- ;
                }
                sum = nums[l]+nums[r]+nums[k] ;
            }
        }
        return tt ;
    }   
};

int main() {
    Solution ss ;
    vector<int>ls ;
    while(1) {
        int a ;
        cin >> a ;
        if(a == 1000) break ;
        ls.push_back(a) ;
    }
    int tar ;
    cin >> tar ;
    int ret = ss.threeSumClosest(ls, tar) ;
    cout << ret << endl ;
    return 0;
}

