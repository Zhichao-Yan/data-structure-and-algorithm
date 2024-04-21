#include <stdlib.h>
#include <stdbool.h>

typedef int ElemType;
typedef struct node{
    ElemType e;
    struct node *left;
    struct node *right;
}Tree;

bool search_tree(Tree *T,ElemType e,Tree **p)
{
    // 树为空，返回错误
    if(T == NULL)
    {
        *p = NULL;
        return false;
    }
    if(T->e == e)
    {
        *p = T;     // 查找成功，使用p返回结点的地址
        return true;
    }
    if(T->left && T->e > e)
    {
        return search_tree(T->left,e,p);
    }
    if(T->right && T->e < e)
    {
        return search_tree(T->right,e,p);
    }
    // 没有找到,返回最后的结点
    *p = T;
    return false;
}

void insert_tree(Tree **T,ElemType e)
{
    // 事先构造结点
    Tree *s = (Tree*)malloc(sizeof(struct node));
    s->e = e;
    s->left = NULL;
    s->right = NULL;
    if(*T == NULL)
    {
        // 如果树本来是空的
        *T = s;
        return;
    }
    Tree *p;
    // 树非空，于是在树中查找
    if(search_tree(*T,e,&p))
    {
        // 已经存在，不用插入
        printf("结点%d已经存在\n",e);
        return;
    }
    // 查找失败，插入
    if(p->e > e)
    {
        // 插入作为左子树结点
        p->left = s;
    }else{
        // 插入作为右子树结点
        p->right = s;
    }
    return;
}

Tree* build_tree(ElemType *data,size_t size)
{   
    Tree * T = NULL;
    for(size_t i = 0; i < size; ++i)
    {
        insert_tree(&T,data[i]);
    }
    return T;
}


void delete(Tree *T,ElemType e,Tree *f)
{
    if(!T)
        return;
    if(T->e == e)
    {
        if()
    }
    if(T->e > e)
    {
        delete(T->left,e,T);
    }
    if(T->e < e)
    {
        delete(T->right,e,T);
    }
    return;
}

int main()
{
    int A[7] = {45,24,53,45,12,24,90};
    Tree * T = build_tree(A,7);
    delete(T,45,NULL);
}