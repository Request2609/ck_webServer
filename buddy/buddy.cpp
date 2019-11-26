#include "buddy.h"

map<int, list<free_block_type>>buddy :: buddy_list ;
map<int, allocated_block>buddy :: process_list ;
vector<free_block_type> buddy :: mem_list ;
int default_max_list_block_size = 1024 ;
int default_size = 4096 ;

int buddy :: id = 0 ;

void buddy :: init() {
    for(int i=0; i<=10; i++) {
        list<free_block_type> tmp ;
        free_block_type fbt ;
        fbt.start_addr = 0 ;
        fbt.size = default_size ;
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
    cout << "请输入进程需要的块大小:" << endl ;
    cin >> pro.size ;
    if(pro.size == 0) {
        return  ;
    }
    //找到合适大小的内存块
    int size = get_near_bigger_block(pro.size) ;
    //重新设置内存的大小
    pro.size = size ;
    cout << "设置进程块的大小" << size<<endl ;
    if(size > default_max_list_block_size) {
        cout << "暂时无法分配该大小的块" << endl ;
        return ;
    }
    int flag = 0 ;
    for(auto fbt = mem_list.begin(); fbt != mem_list.end(); fbt++) {
        if(fbt->size >= pro.size) {
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
            cout << "分配成功！" << endl ;
            return ;
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
    cout << "请输入字节长度:" << endl ;
    auto res=mem_list.begin() ;     
    cin >> res->size ;
    default_size = 4096 ; 
    res->start_addr = 0 ;
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
    //想创建一个内存空闲块
    fbt.size = res->second.size ;  
    fbt.start_addr = res->second.start_addr ;
    cout << "按照地址排序!" << endl ;
    //按照地址进行排序，方便合并
    range_mem_by_addr() ;
    //检查内存空闲块中的伙伴

    for(auto tmp=mem_list.begin(); tmp != mem_list.end(); tmp++) {
        //情况1
        //条件1:当前释放进程的开始地址和所占字节之和等于当前空闲块的起始地址
        //条件2:当前进程的占用的字节大小要等于当前空闲块的大小
        if(tmp->start_addr == 
           res->second.start_addr+res->second.size 
               && tmp->size == res->second.size) {
            //合并操作
            tmp->size *= 2 ;
            tmp->start_addr = res->second.start_addr ;
            range_mem_by_addr() ;
            check_block() ;
            return ; 
        }
        //情况2
        //条件1:当前是空闲块的开始地址和拥有空闲块的大小相加等于进程起始地址
        //条件2:当前空闲块大小等于释放进程的块的大小
        if(tmp->start_addr+tmp->size == res->second.start_addr 
           && tmp->size == res->second.size) {
            tmp->size *= 2 ;
            check_block() ;
            return  ;
        }
    } 
    mem_list.push_back(fbt) ;
    //重新按照地址进行排序
    range_mem_by_addr() ;
    //整理内存块
}

void buddy :: check_block() {
    int a = 0 ;
    while(a == 0) {
        cout << "合并忽略的空闲块" << endl ;
        merge_block(a) ;
    }
}

void buddy :: merge_block(int& a) {
    free_block_type del ;
    del.start_addr = -1 ;
    int len = mem_list.size() ;
    for(int i=0; i<len-1; i++) {
        //当前块的大小与后面紧挨着的块的大小相等，并且当前的块的起始地址加上块的大小等于下面的块的地址
        if(i+1<len && mem_list[i].size == mem_list[i+1].size 
           && mem_list[i].start_addr+mem_list[i].size == mem_list[i+1].start_addr) {
            cout << mem_list[i].start_addr << "      " << mem_list[i+1].start_addr << endl ;
            mem_list[i].size *= 2 ;
            //删除
            del_block(mem_list[i+1]) ;
            return ;
        }
    }
    if(del.start_addr == -1) {
        a = 1 ;
        return ;
    }

}       

void buddy :: del_block(free_block_type& del) {
    for(auto res = mem_list.begin(); res != mem_list.end(); res++) {
        if(res->start_addr == del.start_addr) {
            mem_list.erase(res) ;
            break ;
        }
    }
}

void buddy :: print_mem_used_rate() {
    cout << "内存使用率"<< endl ;
    cout << "开始地址" << setw(20) << "空闲空间" << endl ;
    for(auto res : mem_list) {
        cout << res.start_addr <<setw(20) << res.size << endl ;
    }
    cout << "进程内存使用情况" << endl ;
    cout << "进程ID" << setw(20) << "空间大小"<<setw(20) << "开始地址"<< endl ;
    for(auto res : process_list) {
        cout << res.first << setw(20) << res.second.size << setw(20) << res.second.start_addr << endl ;
    }
}

void buddy :: swap(int& a, int& b) {
    int tmp = a ;
    a = b ;
    b = tmp ;
}

bool compare_addr(const free_block_type& tmp1, const free_block_type& tmp2) { 
    return tmp1.start_addr<tmp2.start_addr ;
}

bool compare_size(const free_block_type& tmp1, const free_block_type& tmp2) {
    return tmp1.size < tmp2.size ;
}

void buddy :: range_mem_by_addr() {
    sort(mem_list.begin(), mem_list.end(), compare_addr) ;
}

void buddy :: range_mem_by_size() {
    sort(mem_list.begin(), mem_list.end(), compare_size) ;
}

void buddy :: resize_mem() {
    int size = 0 ;
    cout << "请输入扩充的内存大小:" << endl ;
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

void buddy :: do_exit() {
       
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

