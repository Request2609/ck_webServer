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
        addtional_color(INVALID) {}
    ~tree_node() {}
private :
    int data ;
    int own_color ;
    int addtional_color ;
    shared_ptr<rb_tree>root ;
    shared_ptr<rb_tree>left ;
    shared_ptr<rb_tree>right ;
    shared_ptr<rb_tree>parent ;
} ;

class rb_tree {
public:
    rb_tree():root(nullptr) {}
    ~rb_tree() {}
public :
    void insert_tree(int info) ;
private :
    void insert_by_bst_way(NODE root, NODE cur) ;
private :
    NODE root ;
};

