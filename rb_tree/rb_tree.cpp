#include "rb_tree.h"

void rb_tree:: insert_tree(int info) {

    shared_ptr<tree_node>cur = make_shared<tree_node>() ;
    cur->data = info ;
    //空树，将该节点给根
    if(root == nullptr) {
        root = cur ;
        root->own_color = BLACK ; 
    } 
    else {
        cur->own_color = RED ;
    }
    //按照二叉搜索树的形式插入
    insert_by_bst_way(root, cur) ;
}

//按照二叉搜索树的形式插入//////////////////////////////
void rb_tree :: insert_by_bst_way(NODE root, NODE cur) {
    
}

