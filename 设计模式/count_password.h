#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <string>
using namespace std ;
//加密适配器

class NewCipher ;
class Caesar ;
class DataOperator ;
class CipherAdapter ;
class NewCipherAdapter ;

class Target {
    
} ;

class DataOperator {
public :
    DataOperator() {}
    virtual ~DataOperator() {}
    virtual string doEncrypt(int key, string ps) {return "" ;};

    void setPassword(string pass) { password = pass ;}
    string getPassword() { return password ;}
private :
    string password ;
} ;


//原来的加密适配器
class CipherAdapter : public DataOperator{
public :

    void setCeasar(shared_ptr<Caesar>cs) {
        cae = cs ;
    }

    CipherAdapter() {}
    string doEncrypt(int key, string ps) ;
private :
    shared_ptr<Caesar> cae ;
} ;

class Caesar {
public :
    string doEncrypt(int key, string ps) {
        cout << "旧的加密适配器" << endl ;
        cout << key << "       " << ps << endl ;
        return to_string(key)+"------->" +ps ;
    }
} ;

//新的加密适配器
class NewCipherAdapter : public DataOperator{
public :
    NewCipherAdapter() {}
    string doEncrypt(int key, string ps) ;
private :
    shared_ptr<NewCipher> nc ;
} ;

class NewCipher {
public :
    string doEncrypt(int key, string ps) {
        cout << "新的适配器" << endl ;
        cout << key <<  "        " << ps << endl ;
        return to_string(key)+"============>"+ps ;
    }

} ;
