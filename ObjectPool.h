#pragma once
#include <iostream>
#include <queue>
#include <memory>
#include <mutex> 
const int defaultObjectNum = 20 ;
//多队列的对象池
template<class T> 
class objectPool {
public:
    objectPool() {
    }
    ~objectPool() {}
    //设置每个队列中的对像数量
    static void setObjectNumber(int number) {
        //要不正在使用的话，可已初始化每个队列的数量，否则初始化无效
        if(isUsing == 0)num = number ;
    }

    //获取对象池对象
    static std::shared_ptr<objectPool>getPool() {
        if(instance == nullptr) {
            instance = std::make_shared<objectPool<T>>();          
        }
        return instance ;
    }

    //设置对象池中的对象队列数量
    void setPoolNum(int qnum) {
        objectPool<T>::objectQueueNum = qnum ;    
        //初始化freeList，将队列加到其中
        for(int i=0; i<qnum; i++) {
            std::queue<std::shared_ptr<T>> tmp ;
            objectPool<T>::freeList.push_back(tmp) ;
            objectPool<T>::allocObject(i) ;
        }
    }

    static void print() {
        int num = objectPool<T>::objectQueueNum ;
        for(int i=0; i<num; i++) {
            std::cout << i<<"号队列"<<"  " << "可用对象数量:" <<freeList[i].size()<<std:: endl ;
        }
    }

    //获取对象
    std::shared_ptr<T> getObject(int index) {
        std::lock_guard<std::mutex>lk(mute) ;
        //第index号队列为空
        if(freeList[index].empty()) {
            std::cout << "当前队列中没创建对象，正在创建对象!" << std::endl ;
            //创建对象
            objectPool<T>::allocObject(index) ;
        }    
        std::cout << "申请对象----->"<< index <<"　　　剩余对象数量:  "<<freeList[index].size()<< std::endl ;
        isUsing = 1 ;
        std::shared_ptr<T> tmp = objectPool<T>::freeList[index].front() ;
        freeList[index].pop() ;
        isUsing = 1 ;
        return tmp ;
    } 

    //为每个队列创建对象
    void createObjectToPool() {
        int num = objectPool<T>::objectQueueNum ;
        for(int i=0; i<num; i++) {
            objectPool<T>::allocObject(i) ;        
        }
    }

    //归还对象
    void returnObject(std::shared_ptr<T>tmp, int index) {
        std:: cout <<"归还对象" << std::endl ;
        std::lock_guard<std::mutex>lk(mute) ;
        objectPool<T>::freeList[index].push(tmp) ;
    } 
private:

    //生成一批对象
    static  void allocObject(int index) {
        //对象不够就创建num个
        if(num == 0) num = defaultObjectNum;
        for(int i=0; i<num; i++) {
            std::shared_ptr<T> tmp =std:: make_shared<T>() ;
            freeList[index].push(tmp) ;
        }    
        std::cout << "创建对象完成!" << "------>" << num << std::endl ;
    }
    //加锁
    std::mutex mute ;
    static int isUsing ;
    //对象池中对象队列数量
    static int objectQueueNum ;
    //每个池子中的对象数量
    static int num ;
    static std::shared_ptr<objectPool<T>> instance ;
    //正在使用的
    static std::vector<std::queue<std::shared_ptr<T>>>freeList ;
    //所有已经申请的对象
} ;

template<class T>
int objectPool<T>::isUsing = 0 ;

template<class T>
int objectPool<T>::objectQueueNum = 0 ;

template<class T>
int objectPool<T>::num = 0 ;

template<class T>
std::vector<std::queue<std::shared_ptr<T>>> objectPool<T> :: freeList ;

template<class T>
std::shared_ptr<objectPool<T>>objectPool<T>:: instance ;
