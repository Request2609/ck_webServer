#include"ThreadPool.h"
//创建线程池
threadPool :: threadPool(unsigned short size) : stop(false){
    
    spareThreadNums = size<1 ? 10: size ;
    
    for(size = 0; size < spareThreadNums; size++) {
        //初始化线程池，使用lambda表达式
        pool.emplace_back(
                          [this]{
                            //当线程没被终止，等待有任务信号
                          while(!this->stop) {
                          //根据信号
                          //准备接收加入到线程池中的任务并执行    
                          std :: function <void ()> task ;
                          {
                          std :: unique_lock<std :: mutex> lock {this->muteLock} ;
                          //等待条件成立
                          this->cond.wait(lock, 
                                          [this]{ 
                                          return this->stop.load()||!this->taskQueue.empty() ;
                                          }) ;
                          //终止线程池操作
                          if(this->stop && this->taskQueue.empty()) {
                          return ;
                          }
                          //这里使用右值引用，实现０赋值拷贝，提高效率
                          task = std :: move(this->taskQueue.front()) ;
                          this->taskQueue.pop() ;
                          }    
                          spareThreadNums -- ;
                          task() ;
                          spareThreadNums++ ;
                          }                   
                          }
        );
    }
}

//销毁线程池
threadPool :: ~threadPool() {
    stop.store(true) ;
    cond.notify_all() ;
    //等待任务都执行完成统一销毁
    for( std :: thread& t : pool) {
        t.join() ;
    }
}

//提交事务
//返回future 类型后，可以将线程执行的任务返回值返回给线程池的线程

//空闲线程数量
int threadPool :: count() {
    return spareThreadNums ;
}

