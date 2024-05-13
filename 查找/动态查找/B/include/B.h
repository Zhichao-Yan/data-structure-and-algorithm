#ifndef B_H
#define B_H

#define m 3
#define yes 1
#define no  0
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef int ElemType;
typedef struct node{

    int key_num; // 关键字数量
    struct node *parent;    // 父结点
    ElemType key[m + 1];    // 结点关键字，0号单元不用
    struct node *ptr[m + 1];    // 结点指针，指向子树
}BTree;

typedef struct{
    struct node *ptr;  // 指向所在的结点
    int i; // 关键字向量位置索引
    int tag;    // 结果
}Result;


Result search(BTree *T,ElemType key);
void insert(BTree **T,ElemType key);
void drop(BTree **T,ElemType key);
void print_node(BTree *p);
#endif