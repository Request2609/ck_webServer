#include <iostream>
#include <thread>
#include <memory>
using namespace std ;


class Node {

} ;

class queue{
public :
    queue(): count(0),val(0),next(NULL),head(NULL){}
    void initQueue() ;
    void push(int tmp) ;
    int pop() ;
    int empty() ;
    int destroy() ;
    int front() ;
public :
    void init() ;
    bool cas(queue** tmp, queue* old, queue* news) ;
    void enQueue(int val) ;
    int deQueue() ; 
    void destroyQ() ;
private :
    int count ;
    int val ;
    queue* next ;
    queue* tail,*head, *dHead ;
} ;

void queue :: destroyQ() {
}

void queue :: init() {
    dHead = head ;
    head = new queue() ;
    head->val = -1 ;
    head->next = NULL ;
    tail = head ;
}


//进队，保持原子性
void queue :: enQueue(int val) {
    queue* t = NULL ;
    queue* qu = new queue() ;
    qu->val = val ;
    qu->next = NULL ;
    while(true) {
        t = tail ;
        queue* next = tail->next ;
        if(tail != t) {
            continue ;
        }
        //要是next不为NULL，则fetch全局变量到next
        if(next != NULL) {
            cas(&tail, t, next) ;
            continue ;
        }
        //如果加入节点成功，则退出
        if(cas(&tail->next, next, qu) == true) {
            break ;
        }
    }
    //设置尾部节点
    cas(&tail, t, qu) ;
}

int queue :: deQueue() {
    queue *qq = head ; 
    queue *cur ;
    do {
        if(head == tail && cas(&head, qq, qq->next) == false) {
            continue ;
        }
        cur = head ;
        if(cur == NULL) {
            return  0 ;
        }
    }while(cas(&head, cur, cur->next) != true) ;
    int t = cur->next->val ;
    delete(cur) ;
    cur = NULL ;
    return t ;
}


bool queue :: cas(queue** tmp, queue* old, queue* newNode) {
    if(*tmp != old) {
        return false ;
    }
    *tmp = newNode ;
    return true ;
}

void queue :: initQueue() {
    queue* tmp ;
    tmp = (queue *)malloc(sizeof(queue)) ;
    tmp->next = NULL ;
    head = tmp ;
    tail = tmp ;
}

int queue :: front() {
    return head->val ;
}

void queue :: push(int tmp) {
    //队列为空的话
    if(tail == head&&count == 0) {
        count ++ ;
        head->val = tmp ;
        return ; 
    }
    queue * node = (queue*)malloc(sizeof(queue)) ;
    node->val = tmp ;
    node->next = tail->next ;
    tail->next = node ;
    tail = node ;
    count ++ ;
}

int queue :: pop() {
    if(count == 0) {
        return -1 ;
    }
    queue * cur = head ;
    int val = head->val ;
    head = head->next ;
    free(cur) ;
    count -- ;
    return val ;   
}

int queue :: empty() {
    if(count == 0) {
        return true ;
    }
    else {
        return false ;
    }
}

int queue :: destroy() {
    while(!empty()) {
        pop() ;
    }  
    return 1 ;
}

queue que ;
int sum = 0 ;

void consumer() {
    for(int i=0; i<10; i++) {
        cout << "加入队列:" << i << endl ;
        que.enQueue(i) ;
    }
}

void producter() {
    int a = que.deQueue();
    cout << "数据:"<<a << endl ;
    while(a != 0) {
        a = que.deQueue() ; 
        sum += a ;
        cout << "数据块:" << a << endl ;
    }
}

int main() {
    que.init() ;
    thread t1(consumer) ;
    thread t2(producter) ;
    t1.join() ;
    t2.join() ;
    cout << sum << endl ;
    /*
    que.enQueue(1) ;
    que.enQueue(2) ;
    que.enQueue(3) ;
    int a = que.deQueue() ;
    while(a != 0) {
        cout << a << endl ;
        a = que.deQueue() ;
    }*/
} 
    
