#include <iostream>
#include <vector>
#include <map>
using namespace std ;

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        map<int, int>ls  ;         
        int len = nums.size() ;
        int l = 0 ;
        int r = l ;
        for(int i=0; i<len; i++) {
            //要是当前元素是重复元素，则记录当前元素后面的元素
            if(ls.find(nums[i]) == ls.end()) {
                //使用l保存i的值
                //元素第一次出现
                ls[nums[i]] = 1 ;
                l = i+1 ;
            }
            else {
                //第一次，初始化r的值为i，第一个重复元素
                if(r==0) {
                    r = i+1 ;   
                }
                while(r<len) {
                    //要是r指向的元素不存在于ls中(也就是之前没出现过)
                    if(ls.find(nums[r]) == ls.end()) {
                        nums[i] = nums[r] ;
                        i -- ;
                        break ;
                    }        
                    else {
                        r++ ;
                    }
                }
                if(r == len) break ;
            }   
        }
        return l;
    }
};

int main() {
    int c;
    vector<int>ls ;
    for(;;) {
        cin >> c ;
        if(c == -1) break ;
        ls.push_back(c) ;
    }
    Solution ss ;   
    cout << ss.removeDuplicates(ls) << endl ;
    return 0;
}

