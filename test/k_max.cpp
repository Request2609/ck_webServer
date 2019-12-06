#include <iostream>
#include <vector>
using namespace std ;

class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        vector<int> ls(nums.size()) ;
        int low = 0 ;
        int high = nums.size()-1 ;    
        int mid = 0 ;
        while(low <= high) {
            mid = partition(low, high, nums) ;
            if(mid == k-1) {
                return nums[mid] ;
            }
            if(mid < k-1) {
                low = mid+1 ;
            }
            if(mid > k-1) {
                high = mid-1 ;
            }
        }
        return -1 ;
    }

    int partition(int low , int high, vector<int>&nums) {
        int left = low+1;
        int right = high ;
        swap(nums[low], nums[(high+low)/2]) ;
        int bound = nums[low] ;
        while(left <= right) {
            while(left < high && nums[left] >= bound) left++ ;
            while(nums[right] < bound) right-- ;
            if(left < right) {
                swap(nums[right--], nums[left++]) ;
            }
            else break ;
        }
        swap(nums[low], nums[right]) ;
        return right ;
    }

    void swap(int& l1, int& l2) {
        int tmp = l1 ;
        l1 = l2 ;
        l2 = tmp ;
    }
};

int main() {
    vector<int> ls ;
    while(1) {
        int a ;
        cin >> a ;
        if(a == -1) break ;
        ls.push_back(a) ;
    }
    Solution ss ;
    int num ;
    cin >> num ;
    cout << ss.findKthLargest(ls, num) << endl ;
    return 0;
}

