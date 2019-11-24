#include "buddy.h"

map<int, list<free_block_type>>buddy :: buddy_list ;
map<int, allocated_block>buddy :: process_list ;
list<free_block_type> buddy :: mem_list ;
int default_max_list_block_size = 1024 ;
int default_size = 4096 ;

int buddy :: id = 0 ;

void buddy :: init() {
    
    //维护11个链表
    for(int i=0; i<=10; i++) {
        list<free_block_type> tmp ;
        free_block_type fbt ;
        fbt.start_addr = 0 ;
        fbt.size = 0 ;
        int size = get_block_size(i) ;
        buddy_list.insert({size, tmp}) ;
    }

    free_block_type cur ;
    cur.start_addr=0 ;
    cur.size = default_size ;
    mem_list.push_back(cur) ;
}

int buddy :: get_block_size(int size) {
    int sum = 1 ;
    for(int i=0; i<size; i++) {
        sum*=2 ;
    }
    return sum ;
}

void buddy :: set_default_size(int size) {
    default_size = size ;
}

int buddy :: get_pid() {
    return id++ ;
}

void buddy :: new_process() {
    list<allocated_block>ls ;
    allocated_block pro ;
    pro.pid = get_pid() ;
    cin >> pro.size ;
    //找到合适大小的内存块
    int size = get_near_bigger_block(pro.size) ;
    //重新设置内存的大小
    pro.size = size ;
    while(1) {
        if(size > default_max_list_block_size) {
            cout << "暂时无法分配该大小的块" << endl ;
            return ;
        }
        int flag = 0 ;
        for(auto fbt = mem_list.begin(); fbt != mem_list.end(); fbt++) {
            if(fbt->size-fbt->start_addr >= pro.size) {
                flag = 1 ;
                pro.start_addr = fbt->start_addr ;
                fbt->start_addr+=pro.size ;
                fbt->size -= pro.size ;
                process_list[pro.pid] = pro ;
                //分配后，正好合适
                if(fbt->size == 0) {
                    flag = 1 ;
                    mem_list.erase(fbt) ;
                }
                return ;
            }
            else {
                cout << "内存空间不够了~~~" << endl ;
                break ;
            }
        }
    }
    cout << "分配内存失败!" << endl ;
}

int buddy :: is_ci_mi(int size) {
    return (size&(size-1)) == 0 ?1:0 ;
}


void buddy :: set_free_block_size() {
    //如果已经分配了
    if(mem_list.size() >= 2) {
        cout << "空闲块已经使用" << endl ;
        return ;
    }
    default_size = 4096 ; 
    auto res=mem_list.begin() ;     
    res->start_addr = 0 ;
    res->size += default_size ;
}

//取消进程
void buddy :: cancer_process() {
    int pid ;
    cout << "请输入进程的id号:" ;
    cin >> pid ;
    auto res = process_list.find(pid) ;
    if(res== process_list.end()) {
        cout << "不存在该编号的进程!" << endl ;
        return ;
    }
    range_block(res) ;
    process_list.erase(res) ;
}

//进程开始的地址
void buddy :: range_block(map<int, allocated_block>::iterator res) {
    //创建新的空闲块
    free_block_type fbt ;
    fbt.size = res->second.size ;  
    fbt.start_addr = res->second.start_addr ;
    //按照地址进行排序
    range_mem_by_addr() ;
    for(auto tmp=mem_list.begin(); tmp != mem_list.end(); tmp++) {
        if(tmp->start_addr == 
           res->second.start_addr+res->second.size 
           && tmp->size == res->second.size) {
            tmp->size += res->second.size ;
            tmp->start_addr = res->second.start_addr ;
            return ; 
        }
        else {
            if(tmp->start_addr+tmp->size == 
                res->second.start_addr&& tmp->size == res->second.size) {    
                tmp->size+=res->second.size ; 
                return ;
            }
        }
    } 
    mem_list.push_back(fbt) ;
    //重新按照地址进行排序
    range_mem_by_addr() ;
    //整理内存块
    int a = 0 ;
    while(a != 0) {
        merge_block(a) ;
    }
}

void buddy :: merge_block(int& a) {
    auto res = mem_list.begin() ;
    while(res != mem_list.end()) {
        auto ret = res ;
        res ++ ;
        if(res != mem_list.end() && 
           res->size == ret->size && 
           ret->start_addr+ret->size== res->start_addr) {
            ret->size *=2 ;
            a = 0 ;
            return ;
        }
    }
    a = 1 ;
}       

void buddy :: print_mem_used_rate() {
    cout << "内存使用率"<< endl ;
    cout << "开始地址" << setw(5) << "空闲空间" << endl ;
    for(auto res : mem_list) {
        cout << res.start_addr <<setw(5) << res.size << endl ;
    }
    cout << "进程内存使用情况" << endl ;
    cout << "进程ID" << setw(5) << "空间大小"<<setw(5) << "开始地址"<<setw(5)<< endl ;
    for(auto res : process_list) {
        cout << res.first << setw(5) << res.second.size << setw(5) << res.second.start_addr << endl ;
    }
}

void buddy :: range_mem_by_addr() {
    auto tmp = mem_list.begin() ;
    while(tmp != mem_list.end()) {
        auto next_ = tmp++ ;
        while(next_ != mem_list.end()) {
            if(next_->start_addr < tmp->start_addr) {
                swap(next_->start_addr, tmp->start_addr) ;
                swap(next_->size, tmp->size) ;
            }
            else next_++ ;
        }
        tmp++ ;
    }
}

void buddy :: range_mem_by_size() {
    auto tmp = mem_list.begin() ;
    while(tmp != mem_list.end()) {
        auto next_ = tmp++ ;
        while(next_ != mem_list.end()) {
            if(next_->size < tmp->size) {
                swap(next_->start_addr, tmp->start_addr) ;
                swap(next_->size, tmp->size) ;
            }
            else next_++ ;
        }
        tmp++ ;
    }
}

void buddy :: resize_mem() {
    int size = 0 ;
    cin >> size ;
    if(size < 0) {
        return ;
    }
    free_block_type fbt ;
    fbt.size = size ;
    fbt.start_addr = default_size ;
    default_size+=size ;
    for(auto res = mem_list.begin(); res != mem_list.end(); res++) {
        if(res->start_addr+res->size == fbt.size) {
            res->size += size ;
            return ;
        }
    }
    mem_list.push_back(fbt) ;
    //重新按照地址进行排序
    range_mem_by_addr() ;
}

int buddy :: get_near_bigger_block(int tmp) {
    //要是2的整数次幂，直接返回
    if(is_ci_mi(tmp)) {
        return tmp ;
    }
    //若果不是二的整数次幂，将其转化成最接近的较大的二的整数次幂
    else {
        tmp-- ;
        tmp|=tmp>>1 ;
        tmp|=tmp>>2 ;
        tmp|=tmp>>4 ;
        tmp|=tmp>>8 ;
        tmp|=tmp>>16 ;
        tmp++ ;
        return tmp;
    }
}
