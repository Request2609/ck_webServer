#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
using namespace std ;
vector<condition_variable>chop ;
vector<mutex>person;
int a[5] = {1, 2, 3, 4, 5} ;

class sem{
public :
    mutex& operator[](long index) {
        if(count >= 10) {
            return mute[0] ;
        }
        count ++ ;
        return mute[index] ;
    }

private :
    int count ;
    mutex mute[5] ;
} ;
void eat() {
    
    unique_lock<mutex>lk(mutex[]) ;
    chop[].wait()  ;     
}

int main() {

    for(int i=0; i<6; i++) {
        condition_variable cond ;
        chop.push_back(cond) ;  
    }
    
    return 0;
}

