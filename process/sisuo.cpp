#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <ctype.h>
using namespace std ;

void thread_worker(void*arg) ;
void critical_section(int thread_num, int i) ;

int main() {
    
    int rtn, i ;
    pthread_t tid = 0 ;
    rtn = pthread_create(&tid, NULL, thread_worker, NULL) ;
    
    return 0;
}

