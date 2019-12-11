#pragma once
#include <iostream>
#include <memory>
using namespace std ;
//附加颜色没有用处
const int INVALID = 0 ;
//节点是黑色
const int BLACK = 1 ;
//节点是红色
const int  RED = 2 ;
//节点是复合颜色
const int ALL = 3 ;

class rb_tree ;
class tree_node ;
class rb_tree ;

typedef shared_ptr<tree_node> NODE ;

class tree_node {
    friend class rb_tree;
public :
    tree_node():
        data(-1),
        own_color(RED),
        addtional_color(INVALID), 
        root(nullptr),
        left(nullptr),
        right(nullptr),
        parent(nullptr) 
    {}

    ~tree_node() {}
private :
    int data ;
    int own_color ;
    int addtional_color ;
    NODE root ;
    NODE left ;
    NODE right ;
    NODE parent ;
} ;

class rb_tree {
public:
    rb_tree():root(nullptr) {}
    ~rb_tree() {}
public :
    void insert_tree(int info) ;
private :
    //右旋
    void right_rotate(NODE cur) ;
    void left_rotate(NODE cur) ;

    void insert_by_bst_way(NODE root, NODE cur) ;
private :
    NODE root ;
};

