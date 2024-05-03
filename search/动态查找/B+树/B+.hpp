#ifndef B_H
#define B_H

typedef int ElemType;
typedef int Record;
#include <iostream>
#include <cmath>

class node{
public:
    node *parent;      // 父结点
    int key_num;        // 关键字和子树数量
    ElemType *keys;    // 结点关键字，0号单元不用
    bool leaf;         // 判断是叶子结点还是内部结点
    int od;
    node(int order = 3, bool type = false):
        od(order),
        key_num(0),
        parent(nullptr),
        keys(new ElemType[order + 1]),
        leaf(type){}
    ~node() { delete[] keys; } // 假设 key 是动态分配的，需要在析构函数中释放  
    int binary_search(ElemType target);
    virtual node* split() = 0;
};

// 作为b+树的内部节点
class inner_node : public node { 
public:
    struct node **chd;     // 指向孩子结点的指针
    inner_node(int order = 3):node(order,false),chd(new node*[order + 1]){}  
    ~inner_node(){ delete[] chd; } // 释放孩子指针数组 
    virtual inner_node* split() override;
    void insert(node *ptr,ElemType up);
};

// 作为b+树的叶节点
class leaf_node : public node { 
public:
    Record **data;        // 记录指针数组
    leaf_node *next;            // 链接的下一个叶子结点 
    leaf_node(int order = 3):node(order,true),data(new Record*[order + 1]),next(nullptr){}  
    ~leaf_node(){ delete[] data; } // 释放记录指针数组
    void insert(Record *r);                     // 插入记录
    void drop(Record *r);                     // 插入记录
    Record* search1(ElemType key);           // 返回记录
    int leaf_node::search2(ElemType key);   // 返回关键字位置
    virtual leaf_node* split() override;
};

class bp_tree
{
public:
    node *root;
    leaf_node *sqt;
    int order;
    bp_tree(int od = 3):root(nullptr),sqt(nullptr),order(od){}
    void insert(Record *r);
    node* search_node(ElemType target);
    Record* search(ElemType target);
    void drop(Record *r);
};

#endif