#include <iostream>
#include <vector>
#include <queue>
using namespace std ;

class LRUCache {
public:
    LRUCache(int capacity) {  
        size = capacity ;
    }
    //每次获取的时候，就讲
    int get(int key) {
        
    }

    void put(int key, int value) {
        que.push({key, value}) ;
    }
public :
    int size;
    queue<pair<int, int>> que ;
};


int main() {
    
    return 0;
}

