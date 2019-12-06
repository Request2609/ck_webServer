#include "mem.h"

void display_menu(){
    printf("\n");
    printf("1 - Set memory size (default=%d)\n", DEFAULT_MEM_SIZE);
    printf("2 - Select memory allocation algorithm\n");
    printf("3 - New process \n");
    printf("4 - Terminate a process \n");
    printf("5 - Display memory usage \n");
    printf("0 - Exit\n");
}
/* 设置当前的分配算法 */
void set_algorithm(){
    int algorithm;
    printf("\t1 - First Fit\n");
    printf("\t2 - Best Fit \n");
    printf("\t3 - Worst Fit \n");
    scanf("%d", &algorithm);
    if(algorithm>=1 && algorithm <=3) ma_algorithm=algorithm;
    //设置当前分配算法
    ma_algorithm = algorithm ;
    //按指定算法重新排列空闲区链表
    rearrange(ma_algorithm); 
}

/*按BF算法重新整理内存空闲块链表*/
void rearrange_BF(){
    //请自行补充
    struct free_block_type *tmp, *work;
    int flag = 0 ;
    tmp = (struct free_block_type*)malloc(sizeof(struct free_block_type)) ;
    tmp->start_addr = -1 ;
    tmp->size = 0 ;
    tmp = free_block ;
    while(tmp!=NULL)
    { 
        flag = 0 ;
        work = tmp->next;
        //相当于链表排序
        while(work!=NULL){
            if(work->size < tmp->size) { /*地址递增*/
                swap(work->start_addr, tmp->start_addr);
                swap(work->size, tmp->size);
                flag = 1 ;
            }
            else            work=work->next;            
        }
        if(flag == 1) break ;
        tmp=tmp->next;
    }   
}
/*按WF算法重新整理内存空闲块链表*/

void rearrange_WF(){
    //请自行补充
    struct free_block_type *tmp, *work;
    int flag = 0 ;
    tmp = (struct free_block_type*)malloc(sizeof(struct free_block_type)) ;
    tmp->start_addr = -1 ;
    tmp->size = 0 ;
    tmp = free_block ;
    while(tmp!=NULL)
    { 
        flag = 0 ;
        work = tmp->next;
        //相当于链表排序
        while(work!=NULL){
            if(work->size > tmp->size) { /*大小递减*/
                swap(work->start_addr, tmp->start_addr);
                swap(work->size, tmp->size);
                flag = 1 ;
            }
            else            work=work->next;            
        }
        if(flag == 0) break ;
        tmp=tmp->next;
    }    
}

/*创建新的进程，主要是获取内存的申请数量*/
int new_process(){

    struct allocated_block *ab;
    int size;
    int ret;
    ab=(struct allocated_block *)malloc(sizeof(struct allocated_block));
    if(!ab) exit(-5);
    ab->next = NULL;
    pid++;
    sprintf(ab->process_name, "PROCESS-%02d", pid);
    ab->pid = pid;

    printf("Memory for %s:", ab->process_name);
    scanf("%d", &size);
    if(size>0) ab->size=size ;
    ret = allocate_mem(ab);  /* 从空闲区分配内存，ret==1表示分配ok*/
    /*如果此时allocated_block_head尚未赋值，则赋值*/
    if((ret==1) &&(allocated_block_head == NULL)){ 
        allocated_block_head=ab;
        return 1;
    }
    /*分配成功，将该已分配块的描述插入已分配链表*/
    else if (ret==1) {
        ab->next=allocated_block_head;
        allocated_block_head=ab;
        return 2;
    }
    else if(ret==-1){ /*分配不成功*/
        printf("Allocation fail\n");
        free(ab);
        return -1;
    }
    return 3;
}


/*按FF算法重新整理内存空闲块链表*/
void rearrange_FF(){
    struct free_block_type *tmp, *work;
    int flag = 0 ;
    tmp = (struct free_block_type*)malloc(sizeof(struct free_block_type)) ;
    tmp->start_addr = -1 ;
    tmp->size = 0 ;
    printf("Rearrange free blocks for FF \n");
    tmp = free_block ;
    while(tmp!=NULL)
    { 
        flag = 0 ;
        work = tmp->next;
        //相当于链表排序
        while(work!=NULL){
            if(work->start_addr < tmp->start_addr) { /*地址递增*/
                swap(work->start_addr, tmp->start_addr);
                swap(work->size, tmp->size);
                flag = 1 ;
            }
            else            work=work->next;            
        }
        if(flag == 1) break ;
        tmp=tmp->next;
    }
}

void swap(int* src, int* dst) {
    int s = *src ;
    *src = * dst ;
    *dst = s ;
}

struct allocated_block* find_process(pid_t pid) {
    struct allocated_block* tmp = allocated_block_head ;
    while(tmp) {
        if(tmp->pid == pid) {
            return tmp ;              
        }
        tmp = tmp->next ;
    }
    return NULL ;
}

/*删除进程，归还分配的存储空间，并删除描述该进程内存分配的节点*/
void kill_process(){
    struct allocated_block *ab;
    int pid;
    printf("Kill Process, pid=");
    scanf("%d", &pid);
    ab=find_process(pid);
    if(ab!=NULL){
        free_mem(ab); /*释放ab所表示的分配区*/
    }
}

/*将ab所表示的已分配区归还，并进行可能的合并*/
int free_mem(struct allocated_block *ab){
    int algorithm = ma_algorithm;
    struct free_block_type *fbt, *work;

    fbt=(struct free_block_type*) malloc(sizeof(struct free_block_type));
    if(!fbt) return -1;
    fbt->size = ab->size;
    fbt->start_addr = ab->start_addr;
    dispose(ab) ;
    /*插入到空闲区链表的头部并将空闲区按地址递增的次序排列*/
    fbt->next = free_block;
    free_block=fbt;
    rearrange(ma_algorithm);
    fbt=free_block;
    while(fbt!=NULL){
        work = fbt->next;
        if(work!=NULL){
            /*如果当前空闲区与后面的空闲区相连，则合并*/
            if(fbt->start_addr+fbt->size == work->start_addr){ 
                fbt->size += work->size;
                fbt->next = work->next;
                free(work);
                continue;
            }
        }
        fbt = fbt->next;
    }
    rearrange(algorithm); /*重新按当前的算法排列空闲区*/
    return 1;
}

/*释放ab数据结构节点*/
int dispose(struct allocated_block *free_ab){
    struct allocated_block *pre, *ab;
    
    if(free_ab == allocated_block_head) { /*如果要释放第一个节点*/
        allocated_block_head = allocated_block_head->next;
        free(free_ab);
        return 1;
    }

    pre = allocated_block_head;  
    ab = allocated_block_head->next;

    while(ab!=free_ab){ 
        pre = ab;  
        ab = ab->next; 
    }
    pre->next = ab->next;
    free(ab);
    return 2;
}
/* 显示当前内存的使用情况，包括空闲区的情况和已经分配的情况 */

int display_mem_usage(){
    struct free_block_type *fbt=free_block;
    struct allocated_block *ab=allocated_block_head;
    int flag = 1 ;
    if(fbt==NULL) {
        flag = 0 ;
    }
    printf("----------------------------------------------------------\n");

    /* 显示空闲区 */
    printf("Free Memory:\n");
    printf("%20s %20s\n", "      start_addr", "       size");
    while(flag == 1 && fbt!=NULL){
        printf("%20d %20d\n", fbt->start_addr, fbt->size);
        fbt=fbt->next;
    }    
    /* 显示已分配区 */
    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n", "PID", "ProcessName", "start_addr", " size");
    while(ab!=NULL){
        printf("%10d %20s %10d %10d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
        ab=ab->next;
    }
    printf("----------------------------------------------------------\n");
    return 0;
}

/*按指定的算法整理内存空闲块链表*/
void rearrange(int algorithm){
    switch(algorithm){
    case MA_FF:  rearrange_FF(); break;
    case MA_BF:  rearrange_BF(); break;
    case MA_WF: rearrange_WF(); break;
    }
}

//重新组织块
void rerange_block() {
    switch(ma_algorithm) {
        case 1:
            rearrange_FF() ;
            break ;
        case 2 :
            rearrange_BF() ;
            break ;
        case 3 :
            rearrange_WF() ;
            break ;
    }
}

/*分配内存模块*/
int allocate_mem(struct allocated_block *ab){
    struct free_block_type *fbt, *pre;
    int request_size=ab->size;
    fbt = pre = free_block;
    while(fbt!=NULL){
        if(fbt->size>=request_size){/*分配后空闲空间足够大，则分割*/
            ab->start_addr = fbt->start_addr ;
            fbt->start_addr += request_size ;
            fbt->size -= request_size ;
            //如果该节点分配后空闲块是0，那就释放掉该节点
            if(fbt->size == 0) {
                if(fbt == free_block) {
                    free_block = fbt->next ;
                    free(fbt) ;
                }
                else {
                    pre->next = fbt->next ;
                    free(fbt) ;
                }
            }
            rerange_block() ;
        }
        else{/*分割后空闲区成为小碎片，一起分配*/
            pre = fbt ;
            fbt = fbt->next ;
            continue ;
        }   
        return 1;
    }
    pre = fbt;
    fbt = fbt->next;
    return -1 ;
}

int set_mem_size(){
    int size;
    if(flag!=0){  //防止重复设置
        printf("Cannot set memory size again\n");
        return 0;
    }
    printf("Total memory size =");
    scanf("%d", &size);
    if(size>0) {
        mem_size = size;
        free_block->size = mem_size;
    }
    flag=1;  
    return 1;
}


struct free_block_type* init_free_block(int mem_size){
    struct free_block_type *fb;

    fb=(struct free_block_type *)malloc(sizeof(struct free_block_type));
    if(fb==NULL){
        printf("No mem\n");
        return NULL;
    }
    fb->size = mem_size;
    fb->start_addr = DEFAULT_MEM_START;
    fb->next = NULL;
    return fb;
}

void do_exit() {

    //释放空闲块链表和进程链表
    struct free_block_type * tmp = free_block ;
    struct free_block_type* pre_tmp = tmp ;
    struct allocated_block* alloc = allocated_block_head ;
    struct allocated_block* pre = allocated_block_head ;
    while(tmp) {
        pre_tmp = tmp ;
        tmp = tmp->next ;
        free(pre_tmp) ;
    }
    free(pre_tmp) ;
    while(alloc) {
        pre = alloc ;
        alloc = alloc->next ;
        free(pre) ;
    }
    free(pre) ;
}

int main(){
    int choice;
    pid=0;
    free_block = init_free_block(mem_size); //初始化空闲区
    for(;;){
        display_menu();	//显示菜单
        cin >> choice;	//获取用户输入
        switch(choice){
        case 1: set_mem_size(); break; 	//设置内存大小
        case 2: set_algorithm();flag=1;  break;	//设置分配算法
        case 3: new_process(); flag=1; break;	//创建新进程
        case 4: kill_process(); flag=1;  break;	//删除进程
        case 5: display_mem_usage(); flag=1;  break;	//显示内存使用
        case 0: do_exit(); exit(0);		//释放链表并退出
        default: break;
        }
    }
}

