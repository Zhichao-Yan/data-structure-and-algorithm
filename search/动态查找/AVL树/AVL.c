#include "AVL.h"

/* 获得结点T的树深度 */
static int get_depth(Tree *T)
{
    if(T)
        return T->dp;
    return 0;
}

/* 更新结点的深度 */
static void set_depth(Tree *T)
{
    int len1 = get_depth(T->left);
    int len2 = get_depth(T->right);
    T->dp = ((len1 > len2)? len1 : len2) + 1;
    return;
}

/* 右旋（由左倾变成右倾） */
static void right_rotate(Tree **T)
{
    Tree *p = *T;
    Tree *lc = p->left;
    p->left = lc->right;
    lc->right = p;
    *T = lc;
    set_depth(p);
    set_depth(lc);
    return;
}

/* 左旋（由右倾变成左倾） */
static void left_rotate(Tree **T)
{
    Tree *p = *T;
    Tree *rc = p->right;
    p->right = rc->left;
    rc->left = p;
    *T = rc;
    set_depth(p);
    set_depth(rc);   
    return;
}

/* 当前左倾，进行右的平衡 */
static void right_balance(Tree **T)
{
    Tree *lc = (*T)->left;
    // 取得rc左右子树的深度来进行判断接下来的操作
    int len1 = get_depth(lc->left);
    int len2 = get_depth(lc->right);
    if(len1 - len2 == 1)
    {
        // 直接右旋进行平衡
        right_rotate(T);
        return;
    }
    if(len1 - len2 == -1)
    {
        // 先左旋，再右旋
        left_rotate(&((*T)->left));
        right_rotate(T);
        return;
    }
}

/* 当前右倾，进行左的平衡 */
static void left_balance(Tree **T)
{
    Tree *rc = (*T)->right;
    // 取得rc左右子树的深度来进行判断接下来的操作
    int len1 = get_depth(rc->left);
    int len2 = get_depth(rc->right);
    if(len1 - len2 == -1)
    {
        // 直接左旋
        left_rotate(T);
        return;
    }
    if(len1 - len2 == 1)
    {
        // 先右旋，再左旋
        right_rotate(&((*T)->right));
        left_rotate(T);
        return;
    }
}

bool insert(Tree **T, ElemType e)
{
    // 插入新生成的结点
    if(!(*T))
    {
        
        *T = (Tree*)malloc(sizeof(struct node));
        (*T)->data = e;
        (*T)->left = NULL;
        (*T)->right = NULL;
        (*T)->dp  = 1;
        return true;
    }
    Tree *tree = *T;
    if(tree->data == e)
    {
        return false;
    }
    if(tree->data > e)
    {
        if(!insert(&tree->left,e))
        {
            return false;
        }else{
            int len1 = get_depth(tree->left);
            int len2 = get_depth(tree->right);
            if(len1 - len2 == 2)
            {
                right_balance(T);
            }else{
                (*T)->dp = ((len1 > len2)? len1 : len2) + 1;
            }
            return true;
        }
    }else{
        if(!insert(&tree->right,e))
        {
            return false;
        }else{
            int len1 = get_depth(tree->left);
            int len2 = get_depth(tree->right);
            if(len1 - len2 == -2)
            {
                left_balance(T);
            }else{
                (*T)->dp = ((len1 > len2)? len1 : len2) + 1;

            }
            return true;
        }
    }
}


void print_tree(Tree *root)
{
    if (!root) {
        return;
    }
    // 初始化队列和节点计数器
    Tree *queue[1000];
    int front = 0, rear = 0;

    // 将根节点入队
    queue[rear++] = root;
    while (front != rear) 
    {
        int count = rear - front;
        for(int i = 0; i < count; ++i)
        {
            Tree *node = queue[front++];
            printf("%d ", node->data);
            if (node->left) 
            {
                queue[rear++] = node->left;
            }
            if (node->right) 
            {
                queue[rear++] = node->right;
            }
        }
        printf("\n");
    }
}

/* 中序遍历二叉排序树 */
void traverse(Tree *T)
{
    if(T)
    {
        if(T->left)
            traverse(T->left);
        printf("%d ",T->data);
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