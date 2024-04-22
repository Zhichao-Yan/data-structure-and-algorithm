#ifndef AVL_H
#define AVL_H


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef int ElemType;
typedef struct node{
    ElemType data;
    int dp;
    struct node *left;
    struct node *right;
}Tree;


bool insert(Tree **T, ElemType e);
void traverse(Tree *T);
void show(Tree *T);
// void print_tree(Tree *root, int level);
// void print_tree(Tree *root);
// void print_tree_bfs(Tree *root);
#endif