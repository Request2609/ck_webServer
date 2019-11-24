#pragma once
#include <iostream>
#include <list>
#include <map>
#include <iomanip>

using namespace std ;
const int process_name_len = 50 ;

//内存空闲区的数据结构
struct free_block_type {
    int status ;
    int size ;
    int start_addr ;
} ;

struct allocated_block{
    int pid;
    int size;
    int start_addr;
    char process_name[process_name_len];
};

class buddy {
public :
    buddy() {}
    ~buddy() {}
public : 
    static int get_pid() ;
    static void init() ;
    static int get_block_size(int size) ;
    static void set_default_size(int size) ;
    static void new_process() ;   
    static int get_near_bigger_block(int tmp) ;
    void cancer_process() ;
    static int is_ci_mi(int size) ;
    static void resize_mem() ;
    static void set_free_block_size() ;
    //按照内存开始地址排序
    static void range_mem_by_addr() ;
    //按照空间大小进行排序
    static void range_mem_by_size() ;
    static void range_block(map<int, allocated_block>::iterator res) ;
    static void print_mem_used_rate() ;
    static void merge_block(int& a) ;
private :   
    
    //内存管理链表
    static int id ;
    static list<free_block_type>mem_list ;
    static map<int, list<free_block_type>>buddy_list ;
    static map<int, allocated_block> process_list ;
} ;
