//source: boost_queue.cpp
//目的： 测试boost无锁队列的使用方法
//操作系统：ubuntu 14.04
//安装boost库命令：sudo apt-get install libboost-all-dev
//pubdate: 2015-1-31   当前boost-dev版本是1.54
//编译命令： g++ boost_queue.cpp -lboost_thread -lboost_system
//boost include位置: /usr/include/boost
//boost lib位置： ls /usr/lib/x86_64-linux-gnu/ | grep 'boost'
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <boost/atomic.hpp>
 
using namespace std;
 
//生产数量
boost::atomic_int producer_count(0);
//消费数量
boost::atomic_int consumer_count(0);
//队列
boost::lockfree::queue<int> queue(512);
 
//迭代次数
const int iterations = 100;
//生产线程数
const int producer_thread_count = 4;
//消费线程数
const int consumer_thread_count = 2;
 
//生产函数
void producer(void)
{
    for (int i = 0; i != iterations; ++i) {
    	//原子计数————多线程不存在计数不上的情况
        int value = ++producer_count;
        cout << "*";     //观察生产类型： 纯生产还是同时有消费的情况
        //若没有进入队列，则重复推送
        while (!queue.push(value)) ;
    }
}
 
//是否生产完毕标志
boost::atomic<bool> done (false);
 
//消费函数
void consumer(void)
{
    int value;
    //当没有生产完毕，则边消费边生产
    while (!done) {
    	//只要能弹出元素，就消费
        while (queue.pop(value)) {
        	cout << ".";     //观察消费类型： 纯消费还是边生产边消费
            ++consumer_count;
        }
    }
	//如果生产完毕，则消费
    while (queue.pop(value))
        ++consumer_count;
}
 
int main(int argc, char* argv[])
{
  /*  cout << "boost::lockfree::queue is ";
    if (!queue.is_lock_free())
        cout << "not ";
    cout << "lockfree" << endl;
 */
	//线程群管理器
    boost::thread_group producer_threads, consumer_threads;
 
	//创建生产者线程
    for (int i = 0; i != producer_thread_count; ++i)
        producer_threads.create_thread(producer);
	//创建消费者线程
    for (int i = 0; i != consumer_thread_count; ++i)
        consumer_threads.create_thread(consumer);
	//等待生产者生产完毕
    producer_threads.join_all();
    //可以消费标志
    done = true;     //主线程不等生产线程完毕就设置done标记
    cout << "done" << endl;    //输出以观察主线程和各子线程的执行顺序
    //等待消费者结束
    consumer_threads.join_all();   //由于消费者数量小于生产者数量，因此，供大于求，后阶段是纯消费期
	//输出生产和消费数量
    cout << "produced " << producer_count << " objects." << endl;
    cout << "consumed " << consumer_count << " objects." << endl;
    
    return 0;
}
