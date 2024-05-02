#include "B+.hpp"

int node::binary_search(ElemType target)
{
    ElemType *keys = this->keys;
    int left = 1,right = this->key_num;
    while (left <= right) 
    {
        int mid = (left + right) / 2;
        if (keys[mid] == target) 
        {
            return mid;  // 找到目标值，返回其索引
        }else if(keys[mid] < target) 
        {
            left = mid + 1;  // 目标值在右半区间，更新左边界
        }else 
        {
            right = mid - 1;  // 目标值在左半区间，更新右边界
        }
    }
    // right < left
    return right;  // 始终返回right 
}

void inner_node::insert(node *ptr,ElemType key)
{
    int i = binary_search(key) + 1;
    for(int k = key_num; k >= i; --k)
    {
        keys[k + 1] = keys[k];
        chd[k + 1] = chd[k];
    }
    keys[i] = key;
    chd[i] = ptr;
    ++key_num;
    return;
}

void leaf_node::insert(Record *r)
{
    ElemType key = *r;  // 得到记录中的关键字
    int i = binary_search(key) + 1;    // 获得插入位置
    for(int k = key_num; k >= i; --k)
    {
        keys[k + 1] = keys[k];
        data[k + 1] = data[k];
    }
    keys[i] = key;
    data[i] = r;
    ++key_num;
    return;
}


inner_node* inner_node::split()
{
    inner_node *ptr = new inner_node(od);
    int k = (int)ceil((double)od/2);
    int start = k + 1; 
    int len = od - k;
    ptr->chd[0] = chd[k];
    ptr->chd[0]->parent = ptr;
    chd[k] = nullptr;
    keys[k] = 0;
    key_num--;
    for(int i = 0; i < len; i++)
    {
        ptr->chd[1 + i] = chd[start + i];
        ptr->chd[1 + i]->parent = ptr;
        ptr->keys[1 + i] = keys[start + i];
        ptr->key_num++;
        chd[start + i] = nullptr;
        keys[start + i] = 0;
        key_num--;
    }
    return ptr;
}

leaf_node* leaf_node::split()
{
    leaf_node *ptr = new leaf_node(od);
    int k = od/2;
    int len = od - k;
    int start = k + 1;
    for(int i = 0; i < len; i++)
    {
        ptr->data[1 + i] = data[start + i];
        ptr->keys[1 + i] = keys[start + i];
        ptr->key_num++;
        data[start + i] = nullptr;
        keys[start + i] = 0;
        key_num--;
    }
    if(next)
        ptr->next = next;
    next = ptr;
    return ptr;
}

void bp_tree::insert(Record *r)
{
    if(root == sqt && root == nullptr)
    {
        leaf_node *ptr = new leaf_node(order);
        ptr->insert(r);
        root = sqt = ptr;
        return;
    }
    ElemType key = *r;  // 得到记录中的关键字
    node *ptr = root;   
    while(ptr && !ptr->leaf )   // ptr存在并且不是叶子结点，而是内部结点
    {
        int loc = ptr->binary_search(key);
        inner_node *p = static_cast<inner_node*>(ptr);
        ptr = p->chd[loc];
    }
    leaf_node *q = static_cast<leaf_node*>(ptr);
    q->insert(r);
    while(ptr)
    {
        if(ptr->key_num <= order - 1)
        {
            break;
        }
        ElemType up;
        int k,start,len;
        if(ptr->leaf)   // 如果是叶子结点
        {
            up = ptr->keys[order/2 + 1];
        }else{
            up = ptr->keys[(int)ceil((double)order/2)];
        }
        // k处关键字是前面结点的最后一个，k+1是后面结点的第一个
        node *qtr = ptr->split();
        if(!ptr->parent)
        {
            inner_node *father = new inner_node(order);
            father->chd[0] = ptr;
            father->keys[1] = up;
            father->chd[1] = qtr;
            father->key_num++;
            ptr->parent = father;
            qtr->parent = father;
            root = father;
        }else{
            inner_node * father = static_cast<inner_node*>(ptr->parent);
            qtr->parent = father;
            father->insert(qtr,up);
        }
        ptr = ptr->parent;
    }
}