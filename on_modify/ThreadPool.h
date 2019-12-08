#pragma once
#include<vector>
#include<iostream>
#include<thread>
#include<atomic>
#include<future>
#include<functional>
#include<condition_variable>
#include<queue>
#include<memory>

class threadPool {
public:
    threadPool() {} 
    threadPool(unsigned short size) ;
    ~threadPool() ;
public :
    template<class F, class... Args> 
        auto commit(F&& f, Args&&... args)-> std :: future<decltype(f(args...))> ;

    void print() { printf("hhhhh") ;}
    int count() ;
private:    
    //定义线程中的函数类型
    using task = std :: function<void()> ;
    //线程池
    std :: vector<std :: thread>pool ;
    //任务队列
    std :: queue<task> taskQueue ;
    //互斥锁
    std :: mutex muteLock ;
    //条件变量
    std :: condition_variable cond ;
    //开启线程池的句柄，atomic保证了操作stopped的原子性
    std :: atomic<bool> stop;
    //空闲线程数量
    std :: atomic<int> spareThreadNums ;
};

template<class F, class... Args> 
auto threadPool :: commit(F&& f, Args&&... args)-> std :: future<decltype(f(args...))> {
    
    if(stop.load()) {
        std::cout << "-----" << std::endl ;
       throw std :: runtime_error("线程池已经停止工作")  ;
    }
    //获取函数返回值类型
    using retType = decltype(f(args...)) ;
   //任意参数的函数/任务转化成同意的void()类型的函数，通过task接收
    auto task = std :: make_shared<std :: packaged_task<retType()>> (
                                                                  std :: bind(std :: forward<F>(f), std :: forward<Args>(args)...)                                                             
                                                                     ) ;
    //先获取future
    std :: future<retType> future ;
    {
        //对当前语句加锁
        std :: lock_guard<std :: mutex> lock{ muteLock } ;
        //将任务加入到队列中
        taskQueue.emplace(
                          [task](){
                          (*task)() ;
                          }
                          ) ;
    }
    cond.notify_one() ;
    return future ;
}
