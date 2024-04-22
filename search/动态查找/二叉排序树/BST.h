#ifndef BST_H
#define BST_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef int ElemType;
typedef struct node{
    ElemType e;
    struct node *left;
    struct node *right;
}Tree;


bool search(Tree *T,ElemType e,Tree **p);
void insert(Tree **T,ElemType e);
Tree* build_tree(ElemType *data,size_t size);
void delete(Tree **T,ElemType e);
void traverse(Tree *T);
void show(Tree *T);

#endif