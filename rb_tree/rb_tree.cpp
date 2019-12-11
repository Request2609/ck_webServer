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

//插入后修正红黑树，保持性质不改变
void rb_tree :: fix_up_insert(NODE cur) {
    auto  parent = cur->parent ;  
    if(parent == nullptr) {
        return ;
    }
    NODE uncle ;
    //当父亲节点的颜色是红色
    while(parent->own_color == RED) {
        //情况1父亲节点和叔叔节点都是红色，
        //则插入点违反了红黑树性质
        //父亲节点不为空，并且要是爷爷节点左孩子
        if(parent->parent != nullptr && 
           parent == parent->parent->left) {
            //记录叔叔节点
            uncle = parent->parent->right ;
        } 
        //父亲节点不为空，并且是爷爷节点的右孩子
        else if(parent->parent != nullptr &&
                parent == parent->parent->right){
            uncle = parent->parent->left ;
        }
        //找到了叔叔节点
        //将叔叔节点和父亲节点都设置成黑色
        if(uncle != nullptr) uncle -> own_color = BLACK ;  
        parent->own_color = BLACK ;
        //将爷爷节点设置成红色
        parent->parent->own_color = RED ;
        //移动cur节点到爷爷节点
        cur = parent->parent ;
        //情况2，z的叔叔节点是黑色，z是右孩子
        //获取叔叔节点
        uncle = nullptr ;
        parent = cur->parent ;
        //判断是父亲节点是左孩子还是右孩子，
        //并找到相应的叔叔节点
        if(parent != nullptr && 
            parent->parent->left == parent) {
            uncle = parent->parent->right ;
        }  

        else if(parent != nullptr && parent->parent->right) {
            uncle = parent->parent->left ;
        }
        if(uncle == nullptr) {
            cout << "什么叔叔节点是空的！" << endl ;
        }
        if(uncle->own_color != BLACK) {
             
        }   
        //将cur设置成parent
        cur = parent ;
        //左旋
        left_rotate(cur->parent) ;
        if() {

        }  
    }   
}   


//左旋
void rb_tree::left_rotate(NODE cur) {
    //获取当前的左孩子
    auto ptr = cur->right ;
    if(ptr == nullptr)  return ;
    //当前节点右子树设置成右孩子的左孩子
    cur->right = ptr->left ;
    if(ptr->left) {
        ptr->left->parent = cur ;
    }   
    //上面解放了右孩子的左指针,
    //这里右子树的左指针重新设置成右子树的父亲节点
    ptr->left = cur ;
    //要是ptr的左孩子不为空的话，将cur设置成该节点的左孩子
    //继承父亲节点  
    ptr->parent = cur->parent ;
    //是根节点
    if(cur->parent == nullptr) root = ptr ;
    //有父亲节点就使父亲节点中的相应指针指向ptr
    else if(cur == cur->parent->left){
        cur->parent->left = ptr ;
    }
    else if(cur == cur->parent->right) {
        cur->parent->right = ptr ;
    }
    //旋转的根节点的父亲节点指针指向新根节点
    cur->parent = ptr ;
    root->own_color = BLACK ;
}


//右旋
void rb_tree::right_rotate(NODE cur) {
    //获取要旋转节点的左节点
    auto ptr = cur->left ;
    //右节点是空，返回
    if(ptr == nullptr) return ;
    //设置要旋转节点的左孩子，左孩子是当前左孩子的右孩子(ptr->right)
    cur->left = ptr->right ;
    if(ptr->right != nullptr) ptr->right->parent = cur ;    
    //上面既然解放了左孩子的右指针
    //这里左孩子的右指针指向cur
    ptr->right = cur ;
    
    //继承父亲节点
    ptr->parent = cur->parent ;
    //要是当前cur parent节点是空
    if(cur->parent == nullptr) root = ptr ;
    //判断是父亲的左孩子还是右孩子
    else if(cur == cur->parent->left) {
        cur->parent->left = ptr ;
    }
    else if(cur == cur->parent->right) {
        cur->parent->right = ptr ;
    }
    cur->parent = ptr ;
    //将当前节点设置成Ptr
    root->own_color = BLACK ;
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


