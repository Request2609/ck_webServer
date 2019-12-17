#include "rb_tree.h"

void rb_tree:: insert_tree(int info) {

    NODE cur = make_shared<tree_node>() ;
    cur->data = info ;
    //空树，将该节点给根
    if(root == nullptr) {
        root = cur ;
        root->own_color = BLACK ; 
        null = make_shared<tree_node>() ;
        null->own_color = BLACK ;   
        //设置root的父亲节点指向null
        root->parent = null ;
        root->left = null ;
        root->right = null ;
        return ;
    } 
    else {
        cur->own_color = RED ;
    }
    //按照二叉搜索树的形式插入
    insert_by_bst_way(cur) ;
    //fix_up_insert(cur) ;
}

//插入后修正红黑树，保持性质不改变
void rb_tree :: fix_up_insert(NODE cur) {
    auto  parent = cur->parent ;  
    //要是当前的父亲节点和祖父节点是空
    if(parent == nullptr&&parent->parent == nullptr) {
        return ;
    }
    NODE uncle ;
    //当父亲节点的颜色是红色
    while(cur->parent != null && cur->parent->own_color == RED) {
        //cout << "cur----->" << cur->data << "    颜色:"  << cur->own_color << endl ;
        //情况1父亲节点和叔叔节点都是红色，
        //则插入点违反了红黑树性质
        //并且要是爷爷节点左孩子
        //当前父亲节点要是爷爷节点的左孩子
        parent = cur->parent ;  
        if(parent == parent->parent->left) {
            //记录叔叔节点
            uncle = parent->parent->right ;
            if(uncle == null) {
                //要是当前父亲节点是红色，修改颜色再自旋
                if(parent->own_color == RED) {
                    parent->parent->own_color = RED ;
                    parent->own_color = BLACK ;
                }
                //对当前祖父节点进行左旋
                right_rotate(parent->parent) ;
            }
            //叔叔节点是不是红色
            else {
                //叔叔节点是红色
                if(uncle -> own_color == RED) {
                    //将叔叔节点和父亲节点设置成红色
                    uncle->own_color = BLACK ;
                    parent->own_color = BLACK ;
                    parent->parent->own_color = RED ;
                    //移动当前插入的节点的指针游标,
                    //因为可能会破坏上面子树的结构
                    cur = parent->parent ;
                }
                else {
                    break ;
                }
            }
            //判断当前节点的父亲节点的父亲节点指向是否为空
            if(cur->parent == null || cur->parent->parent == null) {
                break ;
            } 
            //要是当前游标指向的节点是其父亲节点的右指针指向的位置
            //处于第二种情况
            //获取叔叔节点
            if(cur->parent == cur->parent->parent->left) {
                uncle = cur->parent->parent->right ;
            }
            else {
                uncle = cur->parent->parent->left ;
            }
            if(cur->parent != null && 
               cur == cur->parent->right && uncle->own_color == BLACK) {
                //将游标指向父亲节点
                cur = cur->parent ;
                left_rotate(cur) ;   
            }
            //判断cur节点的爷爷节点是否为空，为空就跳出循环
            //第三种情况，变色加右旋就行
            if(cur->parent == null || cur->parent->parent == null) {
                cur->parent->own_color = BLACK ;
                break ;
            }
            //叔叔节点是黑色，就需要旋转
            if(uncle->own_color == BLACK && cur == cur->parent->left) {
                cur->parent->own_color = BLACK ;
                cur->parent->parent->own_color = RED ;
                //进行右旋
                right_rotate(cur->parent->parent) ;
            }
        }
        ///当前父亲节点是祖父节点的右孩子，和上面的情况相似
        //稍作改动即可
        else {
            uncle = parent->parent->left ;
            if(uncle == null) {
                if(parent->own_color == RED) {
                    parent->parent->own_color = RED ;
                    parent->own_color = BLACK ;
                }
                left_rotate(parent->parent) ;
            }
            else {
                if(uncle->own_color == RED) {
                    uncle->own_color = BLACK ;       
                    parent->own_color = BLACK ;
                    parent->parent->own_color = RED ;
                    cur = parent->parent ;
                }
                else {
                    break ;
                }
            }
            if(cur->parent == null || cur->parent->parent == null) {
                break ;
            }

            if(cur->parent == cur->parent->parent->left) {
                uncle = cur->parent->parent->right ;
            }
            else {
                uncle = cur->parent->parent->left ;
            }
            
            if(cur->parent != null && 
               cur == cur->parent->right && uncle->own_color == BLACK) {
                cur = cur->parent ;
                left_rotate(cur) ;
            }
            if(cur->parent->parent == null) {
                cur->parent->own_color = BLACK ;
                break ;
            }
              
            if(uncle->own_color == BLACK && cur == cur->parent->left) {
                cur->parent->own_color = BLACK ;
                cur->parent->parent->own_color = RED ;
                //进行右旋
                right_rotate(cur->parent->parent) ;
            }
        }
    }
    root->own_color = BLACK ;
}   

NODE rb_tree :: find_node(int num) {
    auto cur = root ;
    while(cur != nullptr) {
        if(num > cur->data) {
            cur = cur->right ;
        }
        else if(num < cur->data) {
            cur = cur->left ;
        }
        else {
            return cur ;
        }
    }
    return nullptr ;
}

//删除节点
void rb_tree :: delete_node(int num) {

    NODE x;
    NODE cur = get_delete_node(num) ;
    while(cur != null) {
        //getchar() ;
        auto tmp = cur ;
        tmp->original_color = cur->own_color ;
        //判断左孩子是否为叶子节点
        if(cur->left == null) {
            x = cur->right ;
            //使用右子树替换当前删除节点
            rb_transform(cur, cur->right) ;
        }
        else if(cur->right == null) {
            x = cur->left ;   
            rb_transform(cur, cur->left) ;
            //cout << cur->data << "   " << cur->left->data << endl ;
            //exit(1) ;
        }
        else {     
            //找右子树中的最小节点找到，插入到删除节点的位置
            //这样右子树都比这个新节点大，左子树都比新节点小
            //维持了二叉搜索树的性质
            tmp = get_minimum(cur->right) ;
            //记下原来的颜色
            tmp->original_color = tmp->own_color ;
            x = tmp->right ;
            if(tmp->parent == cur) {
                x->parent = tmp ;
            }
            //找的节点的父亲节点不是当前删除节点
            else {
                //使得后继节点指向要填补节点的父亲节点
                rb_transform(tmp, tmp->right) ;
                //使用预备填补删除节点位置的节点是指向要删除节点的子树
                tmp->right = cur->right ;
                tmp->right->parent = tmp ;
            }   
            //继承父亲节点
            rb_transform(cur, tmp) ;
            tmp->left = cur->left ;
            tmp->left->parent = tmp ;
            tmp->own_color = cur->own_color ;
        }
        if(tmp->original_color == BLACK) {
            fix_delete_tree(x) ;
        }
        cur = get_delete_node(num) ;
    }
}

NODE rb_tree :: get_delete_node(int num) {
    NODE cur = root ;
    while(cur != null) {
        if(cur->data == num) {
            return cur ;
        }
        else if(cur->data > num) {
            cur = cur->left ;
        }
        else {
           cur = cur->right ; 
        }
    }
    return null ;
}

//删除节点后，进行修复(修复的当前节点是父亲节点的左孩子)
//第一种情况:当前节点是黑+黑且兄弟节点是红色
//第二种情况：当前节点是黑+黑色并且兄弟节点的两个子节点全为黑色
//第三种情况：当前节点颜色是黑+黑色，兄弟节点是黑色，
//兄弟的左子是红色，右子是黑色
//第四种情况：当前节点颜色是黑+黑色，他的兄弟节点是黑色，但是兄弟节点的右子
//是红色，兄弟节点左子是任意色
//修复：当前节点是右孩子（与上面对称）
//第一种情况：当前节点是黑色和黑色当前节点的兄弟是红色
//第二种情况：当前节点是黑色+黑色，并且兄弟及其两个子节点是黑色
//第三种情况：当前节点是黑色+黑色，并且兄弟的右子树的是红色，
//左子树的颜色任意
void rb_tree :: fix_delete_tree(NODE cur) {
    //判断只要传进来的的节点颜色是黑色，加上删除的父亲节点的黑色
    //当前节点是黑+黑色
    while(cur != root && cur->own_color == BLACK) {
        //是当前父亲节点的左子树
        if(cur == cur->parent->left) {
            //获取兄弟节点 
            auto brother = cur->parent->right ;
            //如果兄弟节点是红色----->第一种情况
            if(brother->own_color == RED) {
                //将父亲节点染成红色，将兄弟节点设置成黑色
                cur->parent->own_color = RED ;
                brother->own_color = BLACK ;
                //进行左旋
                left_rotate(cur->parent) ;
                //这个时候兄弟节点已经变成黑色了
                brother = cur->parent->right ;   
            }
            //第二种情况
            //从当前节点和兄弟节点中抽取一重黑色，给父亲节点
            //重新设置当前节点的指
            if(brother->left->own_color == BLACK 
               && brother->right->own_color == RED) {
                brother->own_color = RED ;
                cur = cur->parent ;
            }
            //第三种情况
            //将兄弟节点的染成红色
            //将左孩子染成黑色
            //进行右旋转
            else if(brother->right->own_color == BLACK) {
                brother->own_color = RED ;
                brother->left->own_color = BLACK ;
                right_rotate(brother) ;
                brother = cur->parent->right ;
            }      
            //第四种情况
            //将兄弟节点染成当前父亲节点的颜色，把当前节点父节点
            //染成黑色，兄弟节点右子节点染成黑色。之后以当前节点的
            //父亲节点为支点进行左旋
            brother->own_color = cur->parent->own_color ;
            cur->parent->own_color = BLACK ;
            brother->right->own_color = BLACK ;   
            right_rotate(cur->parent) ;
            cur = root ;
        }
        //当前节点是右子树，参考上面步骤同样的道理
        else {
            //cout << cur->parent->data <<"<+++++"<< endl ;
            auto brother = cur->parent->left ;
            //第一种情况
            //将父亲节点染成红色，将兄弟节点染成黑色
            //并且以父亲节点为支点进行右旋，旋转完成重新设置
            //兄弟节点
            if(brother->own_color == RED) {
                cur->parent->own_color = RED ;               
                brother->own_color = BLACK ;
                right_rotate(cur->parent) ;
                brother = cur->parent->left ;
            }
            //第二种情况，移动当前节点,到父亲节点
            if(brother->left->own_color == BLACK  && 
               brother->right->own_color == BLACK) {
                brother->own_color = RED ;
                cur = cur->parent ;
            } 
            //第三种情况，将兄弟节点颜色设置红色，将右子树染成黑色
            if(brother->right->own_color == BLACK 
               && brother->left->own_color == RED) {
                brother->own_color = RED ;
                brother->right->own_color = BLACK ;
                left_rotate(brother) ;
                brother = cur->parent->right ;
            }
            //将当前兄弟节点设置成父亲节点的颜色，
            //讲父亲节点的颜色设置成黑色
            //将兄弟节点的左节点设置成黑色
            brother->own_color = cur->parent->own_color ;
            cur->parent->own_color = BLACK ;
            brother->left->own_color = BLACK ;
            right_rotate(cur->parent) ;
            cur = root ;
        }   
    }    
    cur->own_color = BLACK ;
}

//找当前出入节点中存的数值最小堆的节点
NODE rb_tree :: get_minimum(NODE cur) {
    auto  tmp = cur ;   
    while(tmp != null) {
        if(tmp->left == null) {
            break ;
        }              
        else {
            tmp = tmp->left ;
        }
    }
    return tmp ;
}

//删除树中的一个节点
void rb_tree :: rb_transform(NODE  cur, NODE son) {
    //判断是不是根节点，是就讲儿子节点设置成根节点
    if(cur->parent == null) {
        son->parent = null ;
        root = cur ;
    }
    //设置当前删除父亲节点的指向
    else if(cur == cur->parent->left){
        cur->parent->left = son ;             
    }
    else if(cur == cur->parent->right){
        cur->parent->right = son ;
    }
    //儿子节点的父亲设置成要删除节点的父亲
    son->parent = cur->parent ;
} 

//左旋
void rb_tree::left_rotate(NODE cur) {
    //获取当前的左孩子
    auto ptr = cur->right ;
    if(ptr == null)  return ;
    cur->right = ptr->left ;
    //当前节点右子树设置成右孩子的左孩子
    if(ptr->left != null) {
        ptr->left->parent = cur ;
    }   
    //上面解放了右孩子的左指针,
    //这里右子树的左指针重新设置成右子树的父亲节点
    //要是ptr的左孩子不为空的话，将cur设置成该节点的左孩子
    //继承父亲节点  
    ptr->parent = cur->parent ;
    //是根节点
    if(cur->parent == null) {
        root = ptr ;
        root->parent = null ;
    }
    //有父亲节点就使父亲节点中的相应指针指向ptr
    else if(cur == cur->parent->left){
        cur->parent->left = ptr ;
    }
    else if(cur == cur->parent->right) {
        cur->parent->right = ptr ;
    }
    ptr->left = cur ;
    //旋转的根节点的父亲节点指针指向新根节点
    cur->parent = ptr ;
}


//右旋
void rb_tree::right_rotate(NODE cur) {
    //获取要旋转节点的左节点
    auto ptr = cur->left ;
    //右节点是空，返回
    if(ptr == null) return ;
    //设置要旋转节点的左孩子，左孩子是当前左孩子的右孩子(ptr->right)
    cur->left = ptr->right ;
    if(ptr->right != null) ptr->right->parent = cur ;    
    //上面既然解放了左孩子的右指针
    //这里左孩子的右指针指向cur
    //继承父亲节点
    ptr->parent = cur->parent ;
    //要是当前cur parent节点是空
    if(cur->parent == null) root = ptr ;
    //判断是父亲的左孩子还是右孩子
    else if(cur == cur->parent->left) {
        cur->parent->left = ptr ;
    }
    else if(cur == cur->parent->right) {
        cur->parent->right = ptr ;
    }
    ptr->right = cur ;
    cur->parent = ptr ;
}

//按照二叉搜索树的形式插入//////////////////////////////
void rb_tree :: insert_by_bst_way(NODE cur) {
    auto tmp = root ;
    while(tmp != null && cur != null) {
        if(tmp->data >= cur->data) {
            if(tmp->left == null) {
                cur->left = tmp->left ;
                tmp->left = cur ;
                cur->parent = tmp ;
                break ;
            }          
            tmp = tmp->left ;
        }   
        else {
            if(tmp->right == null) {
                cur->right = tmp->right ;
                tmp->right = cur ;
                cur->parent = tmp ;
                break ;
            }
            tmp = tmp->right ;
        }
    }
    cur->left = null ;
    cur->right = null ;
    fix_up_insert(cur) ;
   // print_rb_tree() ;
}

void rb_tree :: print_rb_tree() {
    queue<shared_ptr<tree_node>> que ;
    que.push(root) ;
    root->flag = -1 ;
    while(!que.empty()) {
        auto tmp = que.front() ;
        if(tmp->left != null) {
            tmp->left->flag = 1 ;
            que.push(tmp->left) ;
        }
        if(tmp->right != null) {
            tmp->right->flag = 0 ;
            que.push(tmp->right) ;
        } 
        if(tmp->flag == -1) {
            cout << "根节点 " ;
        }
        else if(tmp->flag == 0) {
            cout << "右子树  " ;
        }
        else {
            cout << "左子树  " ;
        } 
        cout << "数据:" << tmp->data << "  " << "颜色:"  ;
        if(tmp->own_color == 1) cout << "黑色" << endl ;
        else cout << "红色" << endl ;
        que.pop() ;
    }
}

