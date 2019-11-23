#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
using namespace std ;

#define MAX 3
unsigned long long main_counter, counter[MAX] ;
void* thread_worker(void*) ;
int main() {
    
    char ch ;
    pthread_t tid[MAX] = {0} ;
    for(int i=0; i<MAX; i++) {
        int* num = (int*)malloc(sizeof(int)) ;
        *num = i ;
        pthread_create(&tid[i], NULL, thread_worker, num) ;
        //填写代码，创建一个普通线程
    }
    //线程执行的函数是thread_worker，并将i作为参数传给线程
    //用户按一次回车执行下面代码
    do{
        unsigned long long sum = 0 ;
        //求所有线程counter的和
        for(int i=0; i<MAX; i++) {
            sum+=counter[i] ;
            printf("%llu\n", counter[i]) ;
        }
        printf("%llu, %llu\n", main_counter, sum) ;
    }while((ch = getchar()) != 'q') ;
    return 0;
}

void* thread_worker(void*p) {
    //这里填写代码，将p复制给thread_num
    int thread_num = *(int*)p ;
    cout << "标号:"<< thread_num << "     线程ID:" << pthread_self() << endl ;
    for(;;) {
        counter[thread_num]++ ; 
        main_counter++ ;
    }
}
