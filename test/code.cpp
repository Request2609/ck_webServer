#include <iostream>


//两种cas的实现
int compare_and_swap(int* reg, int oldval, int newval) {
    int old_reg = *reg ;
    if(old_reg == oldval) {
        *reg = newval ;
    }
    return old_reg ;
}

bool compare_and_swaps(int* addr, int oldval, int newval) {
    if(*addr != oldval) {
        return false ;
    }
    else {
        *addr = newval ;
        return true ;
    }
}

//原子累加
int fetch_and_add(int* reg, int inc) {
    int value = *reg ;
    *reg = value+inc ;
    return value ;
}

//gcc中的实现机制
static inline int fetch_and_adds(int* variable, int value)
{
    __asm__ volatile("lock; xaddl %0, %1"
                     : "+r" (value), "+m" (*variable) // input+output
                     : // No input-only
                     : "memory"
                    );
    return value;
}

//test和set实现机制
int test_and_set(int old_value, int set_value) {
    int flag = old_value ;
    old_value = set_value ; 
    return flag ;
}

void consumer() {
    while(1) {

    }
}

void product() {

}

int main() {
    
    return 0;
}

