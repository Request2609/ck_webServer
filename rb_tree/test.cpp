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

class tree_node ;
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
class tree_node ;
typedef shared_ptr<tree_node> NODE ;
class tree_node1 {
    friend class rb_tree;
public :
    tree_node1():
        data(-1),
        own_color(RED),
        addtional_color(INVALID), 
        root(nullptr),
        left(nullptr),
        right(nullptr),
        parent(nullptr) 
    {}

    ~tree_node1() {}
private :
    int data ;
    int own_color ;
    int addtional_color ;
    tree_node* root ;
    tree_node* left ;
    tree_node* right ;
    tree_node* parent ;
} ;



int main()
{
    cout << sizeof(tree_node1) << endl ;
    std::cout << sizeof(tree_node) << std::endl;
    return 0;
}

