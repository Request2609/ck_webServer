#pragma once
#include <iostream>
#include <vector>
using namespace std ;

#define PROCESS_NAME_LEN 32        /*进程名称的最大长度*/
#define MIN_SLICE    10            /*最小碎片的大小*/
#define DEFAULT_MEM_SIZE 1024       /*默认内存的大小*/
#define DEFAULT_MEM_START 0        /*默认内存的起始位置*/

/* 内存分配算法 */
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3

//指向内存中空闲块链表的首指针
struct free_block_type* free_block ;
//进程内存快链表的首指针
struct allocated_block* allocated_block_head ;

int mem_size=DEFAULT_MEM_SIZE;        /*内存大小*/
int ma_algorithm = MA_FF;            /*当前分配算法*/
static int pid = 0;                    /*初始pid*/
int flag = 0;                         /*设置内存大小标志*/

//内存空闲区的数据结构
struct free_block_type {
    int size ;
    int start_addr ;
    struct free_block_type* next ;
} ;

struct allocated_block{
    int pid;
    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    struct allocated_block *next;
};


void display_menu() ;
void set_algorithm() ;
void rearrange_BF() ;
void rearrange_WF() ;
int new_process() ;
void rearrange_FF() ;
void swap(struct free_block_type** tmp, struct free_block_type** ss) ;
void rerange_block() ;
void kill_process() ;
int free_mem(struct allocated_block *ab) ;

int dispose(struct allocated_block *free_ab) ;
int display_mem_usage() ;
void rearrange(int algorithm) ;
void do_exit() ;
int allocate_mem(struct allocated_block *ab) ;
int set_mem_size() ;
struct free_block_type* init_free_block(int mem_size) ;

