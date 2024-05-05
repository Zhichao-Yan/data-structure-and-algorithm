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
    virtual void merge(node *ptr) = 0;
    virtual void borrow(node *ptr) = 0;
};

// 作为b+树的内部节点
class inner_node : public node { 
public:
    struct node **chd;     // 指向孩子结点的指针
    inner_node(int order = 3):node(order,false),chd(new node*[order + 1]){}  
    ~inner_node(){ delete[] chd; } // 释放孩子指针数组 
    virtual inner_node* split() override;
    virtual void merge(node *ptr) override;        // 把右边索引结点合并到当前索引结点中
    virtual void borrow(node *ptr) override;

    void insert(ElemType key,node *ptr);    // 插入关键字和结点指针到索引结点
    void insert(ElemType key,node *ptr,int i);  // 插入关键字和结点指针到索引结点i位置
    void drop(unsigned int i);
};

// 作为b+树的叶节点
class leaf_node : public node { 
public:
    Record **data;        // 记录指针数组
    leaf_node *next;            // 链接的下一个叶子结点 
    leaf_node(int order = 3):node(order,true),data(new Record*[order + 1]),next(nullptr){}  
    ~leaf_node(){ delete[] data; } // 释放记录指针数组
    virtual leaf_node* split() override;                // 裂解叶子结点
    virtual void merge(node *ptr) override;             // 把右边叶子结点合并到当前叶子结点中
    virtual void borrow(node *ptr) override;

    void insert(Record *r);                 // 往叶子结点插入记录
    void insert(ElemType key, Record *r);   // 往叶子结点中插入关键字和记录
    void insert(ElemType key, Record *r, int i); // 将关键字和记录指针插入在i位置
    void drop(ElemType key);                // 从叶子结点删除关键字代表的记录
    void drop(Record *r);                   // 删除记录r
    void drop(unsigned int i);              // 删除位置i处的关键字和记录
    Record* search(ElemType key);           // 返回记录
};

class bp_tree
{
public:
    node *root;
    leaf_node *sqt;
    int order;
    bp_tree(int od = 3):root(nullptr),sqt(nullptr),order(od){}
    void insert(Record *r);     // 关键字包含在记录中
    void insert(ElemType key,Record *r);    // 插入关键字和其相关的记录指针
    void drop(Record *r);       // 关键字包含在记录中
    void drop(ElemType key);    // 删除关键字和其关联的记录指针
    Record* search(ElemType target);    // 搜索关键字代表的记录
    void traverse();    // 遍历叶子结点
private:
    void check1(node *ptr);     // 检查结点的关键字个数是否超过限制
    void check2(node *ptr);     // 检查结点的关键字个数是否少于限制
    node* search_node(ElemType target);
};

#endif