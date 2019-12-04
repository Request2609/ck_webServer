#pragma once
#include <iostream>
#include <memory>
using namespace std ;

class SmallPen ;
class BigPen ;
class MiddlePen ;
class Color ;

class Pen {
public :
    Pen() {}
    virtual ~Pen() {}
    void getPenColor(int flag) ;
    virtual void draw() = 0;
protected :
    shared_ptr<Color>col ;
} ;

class SmallPen : public Pen {
public:
    void draw() ;
} ;

class BigPen : public Pen {
public:
    void draw() ;
}; 

class MiddlePen : public Pen {
public :
    void draw() ;
};

class Color {
public :
    Color() {}
    virtual ~Color(){}
    virtual void bepaint() = 0;
};

class Red : public Color {
public :
    void bepaint() {
        cout << "红笔" << endl ;
    }
};

class Blue : public Color{
public :
    void bepaint() {
        cout << "蓝笔" << endl ;
    }
} ;

class Black : public Color{
public :
    void bepaint() {
        cout << "黑笔" << endl ;
    }
} ;

