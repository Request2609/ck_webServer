#include "rb_tree.h"

void rb_tree:: insert_tree(int info) {

    NODE cur = make_shared<tree_node>() ;
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
    //从cur开始调整
    
}

//右旋
void rb_tree::right_rotate(NODE cur) {
    //获取当前的左孩子
    auto ptr = cur->right ;
    if(ptr == nullptr)  return ;
    //继承父亲节点  
    ptr->parent = cur->parent ;
    //是根节点
    if(cur->parent == nullptr) root = ptr ;
    //当前节点右子树设置成右孩子的左孩子
    cur->right = ptr->left ;
    //当前右子树的左孩子设置成右子树的左孩子
    ptr->left = cur ;
    ///////////////////////////////////////..........
}

//左旋
void rb_tree::left_rotate(NODE cur) {

}

//按照二叉搜索树的形式插入//////////////////////////////
void rb_tree :: insert_by_bst_way(NODE cur_root, NODE cur) {
    if(cur_root == nullptr) {
        return ;
    }
    if(cur_root->data < root->data)  {
        //要是当前根节点的左孩子是空的
        //将当前cur节点插入到root的左子树中
        if(cur->left == nullptr) {
            cur_root->left = cur ;
            cur->parent = cur_root ;
        }
        //否则继续向下找
        else 
            insert_by_bst_way(cur_root->left, cur) ;   
    }   
    //要是小于或等于当前root节点的数据项
    if(cur->data >= cur_root->data) {
        //根节点右孩子为空
        if(cur_root->right == nullptr) {
            cur_root->right = cur ;
            //设置父亲节点为当前根节点
            cur->parent = cur_root ;
        }
        //否则的话，向右子树继续遍历
        else {
            insert_by_bst_way(cur_root->right, cur) ;     
        }
    }
}

