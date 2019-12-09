#include <iostream>
#include <vector>
#include <algorithm>
using namespace std ; 
class Solution {
public :
    vector<vector<int>>fourSum(vector<int>&num, int target) {
        vector<vector<int>>result ;
        if(num.size() < 4) {
            return result ;
        }   
        int size = num.size() ;
        //从小到大排序
        sort(num.begin(), num.end()) ;
        for(int i=0; i<size-3; i++) {
            //不存在
            if(target <=0 && num[i] > 0)  {
                break ;
            }    
            //经排序后，前四个数之和都比这跟目标数大，条件不成立
            if(num[i]+num[i+1]+num[i+2]+num[i+3] > target) break ;
            //第一个数与最大的三个数之和都比目标数小，就移动最左边的i
            if(num[i]+num[size-3]+num[size-2]+num[size-1] < target) continue ;
    
            //重复项的话就跳过
            if(i>0 && num[i] == num[i-1])continue ;

            //j指针从i的下一个数开始
            for(int j=i+1; j<size-2; j++) {
                //不存在
                if(num[i]+num[j]+num[j+1]+num[j+2] > target) break ;
                //小于目标数就将j指针往右移动
                if(num[i]+num[j]+num[size-2]+num[size-1] < target) continue ;
                int start = j+1 ;
                int end = size-1 ;

                while(start < end) {
                    int sum = num[i]+num[j]+num[start]+num[end] ;
                    if(sum < target)start++ ;
                    else if(sum > target) end-- ;
                    else {
                        result.push_back({num[i], num[j], num[start], num[end]}) ;
                        int last_start = start ;
                        int last_end = end ;
                        //删除重复项
                        while(start < end && num[start] == num[last_start])start++ ;
                        while(start > end && num[end] == num[last_end]) end -- ;
                    }
                }
            }
        }
        return result ;
    }
} ;
int main()
{
    std::cout << "Hello world" << std::endl;
    return 0;
}

