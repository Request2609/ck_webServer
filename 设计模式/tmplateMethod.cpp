#include <iostream>
#include <memory>
using namespace std ;
//需求描述:对于数据库的操作一般是包含打开，链接，操作，关闭
//在这些步骤中，各个数据库的链接有区别，其他操作都是一致的，可以使用
//模板方法减少代码的复用性

class abstractDB{
public :
    abstractDB() {}
    virtual ~abstractDB() {}
    void connDb() { 
        cout << "连接数据库!" << endl ;
    }
    void openDB() { 
        cout << "数据库打开了" << endl ;
    }
    void useDB(){
        cout << "使用数据库!" << endl ;
    }
    virtual void closeDB() {
        
    }
} ;

class mysql : public abstractDB{
public :
    mysql() {}
    virtual ~mysql() {}
    void closeDB() {
        cout << "关闭mysql数据库!" << endl ;    
    }
} ;

class sqlServer: public abstractDB{
public :
    sqlServer() {}
    ~sqlServer() {}
    void closeDB() {
        cout << "关闭sqlServer数据库!" << endl ;
    }
} ;

int main() {
    shared_ptr<abstractDB> db = make_shared<mysql>() ;
    db->connDb() ;
    db->openDB() ;
    db->useDB() ;
    db->closeDB() ;
    db = make_shared<sqlServer>() ;
    db->connDb() ;
    db->openDB() ;
    db->useDB() ;
    db->closeDB() ;
    return 0;
}

