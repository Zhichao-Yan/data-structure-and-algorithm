#ifndef B_H
#define B_H

#define m 3
typedef int ElemType;
typedef int Record;
#include <iostream>


class node{
    int key_num;        // 关键字和子树数量
    node *parent;      // 父结点
    ElemType key[m + 1];    // 结点关键字，0号单元不用
};

// 作为b+树的内部节点
class inner_node : public node { 
    struct node *chd[m + 1];     // 指向孩子结点的指针
};

// 作为b+树的叶节点
class leaf_node : public node { 
    Record* data[m + 1];        // 记录指针数组
    leaf_node *next;            // 链接的下一个叶子结点
};

class bp_tree
{
public:
    node *root;
    leaf_node *sqt;
    int order;
    bp_tree(int od = 3):root(nullptr),sqt(nullptr),order(od){};
private:
    void insert(bp_tree &T,Record *r);
};

#endif