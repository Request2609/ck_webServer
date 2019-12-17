#include <iostream>
#include <algorithm>
#include <vector>
//安排机器
using namespace std ;

struct node {
    int hard_level ;
    int task_time ;
};

void func() {
    
}

bool cmp(const node& n1, const node& n2) {
    return n1.task_time>n2.task_time ;
}
bool cmp1(const node& n1, const node& n2) {
    return n1.hard_level>n2.hard_level ;
}
//贪心算法
int main() {
    int num_mech, num_task ;
    //输入任务数量
    cin >> num_mech >> num_task ;
    //每台机器的最大工作时间和机器等级
    int max_time, level ;
    cin >> max_time >> level ;
    vector<node>task_list_s_time ;
    vector<node>task_list_s_level ;
    for(int i=0; i<num_task; i++){
        //任务需要的时间和任务难度等级
        node no ;
        cin >> no.task_time >> no.hard_level ;
        task_list_s_time.push_back(no) ;
    }
    sort(task_list_s_time.begin(), task_list_s_time.end(), cmp) ;
    sort(task_list_s_level.begin(), task_list_s_level.end(), cmp1) ;
    for(int i=0, j=0; i<num_task; i++) {
        while(j<n && )
    }
    return 0;
}

