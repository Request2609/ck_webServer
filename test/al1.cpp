#include <iostream>

using namespace std;

bool isMagicArr(int index, int sum, int* nums) 
{
	if (sum == 0)		// 递归结束条件1
		return true;	
	if (sum < 0 || index < 0)		// 递归条件结束2， 累加和超过了或者数组中没有值了
		return false;
	// num[index]是sum中的一部分或者不是sum中的一部分
	return isMagicArr(index - 1, sum - nums[index], nums) || isMagicArr(index - 1, sum, nums);
}

bool checkMagic(int num)
{
	int nums[11];			// 把数字num按位存到nums中
	int index = 0;
	int sum = 0;			// 每位数字求和
	for ( ; num > 0; index++)
	{
		nums[index] = num % 10;		// 获取并保存每位数字 
		sum += nums[index];			// 每位数字累计求和
		num = num / 10;				// 除去个位数字
	}
	// 每位数和是奇数，则肯定不是神奇数
	if (sum % 2 != 0)
		return false;

	return isMagicArr(index, sum / 2, nums);
}

int main()
{
	int l = 0, r = 0;
		cin >> l >> r;
        int start = clock() ;
		int count = 0;
	    for (int num = l; num <= r; num++)
		{
			if (checkMagic(num)){
			    cout <<num <<endl ;
                count++;
		    }
        }
		cout << "神奇数总数："<<count << endl;
        int end =clock() ;
        cout <<"总耗时："<<end-start<<endl ;
        
    return 0;
}

