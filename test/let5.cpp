#include <iostream>
using namespace std ;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

void print(ListNode* head);
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode * head = NULL;
        ListNode * tmp = head ;
        head = (ListNode*)malloc(sizeof(ListNode)) ;
        head->next = NULL ;
        ListNode* cur = head ;
        cur = head ;
        ListNode* h1 = l1 ;
        ListNode* h2 = l2 ;
        int flag = 0 ;
        int s = 0 ;
        int k = 1 ;
        int sum = 0 ;
        while(h1 == NULL&&h2 == NULL) { 
            tmp->next = NULL ;
            if(flag == 1)  {
                s = sum/10 ; 
                flag = 0 ;
            }
            if(h1 != NULL && h2 != NULL) {
                sum = h1->val+h2->val+s ;
            }
            if(h1== NULL || h2 == NULL) {
                if(h1 != NULL)
                    sum = h1->val ;
                else sum = h2->val ;
            }
            if(sum < 10) {
                tmp ->val = sum ;
            }
            else {
                tmp->val = sum%10 ;    
                flag = 1 ;
            }
            if(k == 1) {
                cur->val = sum ;
                k = 0 ;
            }
            else {
                ListNode* node = (ListNode*)malloc(sizeof(ListNode)) ;
                node->val = sum ;
                node->next = cur->next ;
                cur->next = node ;
                cur = cur->next ;
            }
            
        }
        ListNode* h = head ;
        while(h) {
            cout << h->val << endl ;
            h = h->next ;
        }
        return head ;
    }
};

void createHead(ListNode* head) {
    head->next = NULL ;
    ListNode* tmp = head; 
    ListNode *cur ;
    int f = 0 ;
    while(1) {
        int a ;
        cin >> a ;
        if(a == -1) {
            break ;
        }
        if(f == 0) {
            tmp->val = a ;
            cur = head ;
            f = 1 ;
        }
        else {
            ListNode* tt = (ListNode*)malloc(sizeof(ListNode)) ;
            tt->val = a ;
            tt->next = cur->next ;
            cur->next = tt ;
            cur = tt ;
        }
    }
}

void print(ListNode* head) {
    ListNode* h = head ;
    while(h) {
        cout << h->val << endl ;
        h = h->next ;
    }
}

void freeNode(ListNode* h) {

    ListNode* ls = h ;
    ListNode* pre = h ;
    while(ls) {
        ls = ls->next ;
        free(pre) ;
        pre = ls ;
    }
    
}

int main() {
    ListNode * head = (ListNode*)malloc(sizeof(ListNode)) ;
    ListNode * head1 = (ListNode*)malloc(sizeof(ListNode)) ;
    createHead(head) ;
    createHead(head1) ;
    freeNode(head) ;
    Solution ss ;
    head= ss.addTwoNumbers(head, head1) ;
    
    return 0;
}


