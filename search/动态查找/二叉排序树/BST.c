#include "BST.h"

/* 查找值为e的结点 */
bool search(Tree *T,ElemType e,Tree **p)
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
        return search(T->left,e,p);
    }
    if(T->right && T->e < e)
    {
        return search(T->right,e,p);
    }
    // 没有找到,返回最后的结点
    *p = T;
    return false;
}

/* 插入值为e的结点 */
void insert(Tree **T,ElemType e)
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
    if(search(*T,e,&p))
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

/* 搭建二叉排序树 */
Tree* build_tree(ElemType *data,size_t size)
{   
    Tree * T = NULL;
    for(size_t i = 0; i < size; ++i)
    {
        insert(&T,data[i]);
    }
    return T;
}

/* 替代策略，不会直接删除结点T */
void replace(Tree *T)
{
    if(T->left)
    {
        Tree *p = T,*s = T->left;
        while(s->right)
        {
            p = s;
            s = s->right;
        }
        T->e = s->e;
        if(p == T)
            T->left = s->left;
        else
            p->right = s->left;
        free(s);
        return;
    }
    if(T->right)
    {
        Tree *p = T,*s = T->right;
        while(s->left)
        {
            p = s;
            s = s->left;
        }
        T->e = s->e;
        if(p == T)
            T->right = s->right;
        else
            p->left = s->right;
        free(s);
        return;
    }
    return;
}

/* 删除结点 */
void delete_node(Tree *T,ElemType e,Tree *f)
{
    // T不存在，直接返回
    if(!T)
        return;
    if(T->e == e)
    {
        if(T->left || T->right)
            replace(T);     // 采用替代策略，删除替代的前驱或者后继
        else
        {
            // 没有左子树也没有右子树，找不到前驱和后继，直接删除T
            free(T);
            // 不用关心T是f的左子树还是右子树
            f->left = NULL;
            f->right = NULL;
        }
    }
    if(T->e > e)
    {
        delete_node(T->left,e,T);
    }
    if(T->e < e)
    {
        delete_node(T->right,e,T);
    }
    return;
}

/* 删除树中值为e的结点 */
void delete(Tree **T,ElemType e)
{
    Tree *tree = *T;
    if(tree->e == e)    // 删除的结点刚刚好是根结点
    {
        if(tree->left || tree->right)
        {
            replace(tree);      // 替代策略
        }else{
            // tree是根结点，并且它没有左子树和右子树
            free(tree);     // 直接释放它
            *T = NULL;      // 并且置空
        }
    }else{
        // 删除结点不是根结点，即它必定有父结点
        if(tree->e < e)         // 去右子树
            delete_node(tree->right,e,tree);
        else                    // 去左子树
            delete_node(tree->left,e,tree);
    }
    return;
}

/* 中序遍历二叉排序树 */
void traverse(Tree *T)
{
    if(T)
    {
        if(T->left)
            traverse(T->left);
        printf("%d ",T->e);
        if(T->right)
            traverse(T->right);
    }
    return;
}
/* 打印遍历结结果 */
void show(Tree *T)
{
    traverse(T);
    printf("\n");
}