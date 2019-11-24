#include <iostream>
#include <vector>
#include <algorithm>

using namespace std ;

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int N = nums.size();
        vector<vector<int> > res;
        cout << N << endl ;
        for (int i = 0; i < N - 2; ++i) {
            //要是起始元素大于0，表示后面元素不可能再有和为０的三个元素了
            if (nums[i] > 0) break;
            //当当前的元素和下一个元素相等，则将指针移到下一个元素
            //因为之前的已经计算过了
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            int l = i + 1;
            int r = N - 1;
            //计算以数组中第i个元素开始以后三个元素之和为０的序列
            while (l < r) {
                //计算三个指针指向的三个数组元素的和
                int s = nums[i] + nums[l] + nums[r];
                //三个数的和大于0，则移动右指针
                if (s > 0) {
                    --r;
                    //小于０移动左指针
                } else if (s < 0) {
                    ++l;
                    //等于0就将三个元素放数组中
                } else {
                    res.push_back({nums[i], nums[l], nums[r]});
                    //移动左指针
                    // while (l < r && nums[l] == nums[++l]);
                    //l小于r并且左边界和相邻元素的值相等的时候，就一直循环递增
                    while (l < r && nums[l] == nums[l+1]&&++l<r){
                        l++ ;
                    }
                    //右边相邻的元素相等就一直递
                    while (l < r && nums[r] == nums[r-1]&&--r>l) {
                        r-- ;
                    }
                }
            }
        }
        return res;
    }
};

int main() {
    vector<int>ls ;
    while(1) {
        int a ;
        cin >> a ;
        if(a == 12)break ;
        ls.push_back(a) ;
    }
    Solution ss ;
    vector<vector<int>> as = ss.threeSum(ls) ;
    int row = as.size() ;
    for(int i=0; i<row; i++) {
        for(int j=0; j<3; j++) {
            cout << as[i][j] << "  " ;
        }
        cout << endl ;
    }
}
