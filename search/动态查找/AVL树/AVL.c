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
    // 重置深度
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
    // 重置深度
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

/* 搜索值为e的树根结点 */
Tree *search(Tree *T,ElemType e)
{
    if(!T)
        return NULL;
    if(T->data == e)
    {
        return T;
    }else if(T->data > e)
    {
        return search(T->left,e);
    }else{
        return search(T->right,e);
    }
    return NULL;
}

/* 替代策略，不会直接删除结点T,
*  而是找到它的前驱结点s，然后复制给结点T，相当于间接删除T。
*  然后删除结点s,返回s的父结点
*/
static Tree* replace(Tree *T)
{
    Tree *p,*s;
    if(T->left)
    {
        p = T;
        s = T->left;
        while(s->right)
        {
            p = s;
            s = s->right;
        }
        T->data = s->data;
        if(p == T)
            p->left = s->left;
        else
            p->right = s->left;
        free(s);
    }else if(T->right)
    {
        p = T;
        s = T->right;
        while(s->left)
        {
            p = s;
            s = s->left;
        }
        T->data = s->data;
        if(p == T)
            p->right = s->right;
        else
            p->left = s->right;
        free(s);
    }
    return p;
}

/* 
如果返回NULL，表示不存在值为e的结点，删除失败，
否则返回非NULL，其为实际被释放结点的父结点 
*/
static Tree *delete_node(Tree *T,ElemType e,Tree *f)
{
    // T不存在，直接返回NULL,说明没有值为e的结点，直接退出返回
    if(!T)
        return NULL;
    if(T->data == e)
    {
        if(T->left || T->right)
            return replace(T);     // 采用替代策略，删除替代的前驱或者后继
        else
        {
            if(f->left == T)
                f->left = NULL;
            else
                f->right = NULL;
            // 没有左子树也没有右子树，找不到前驱和后继，直接删除T
            free(T);
            return f;
        }
    }
    if(T->data > e)
    {
        return delete_node(T->left,e,T);
    }
    if(T->data < e)
    {
        return delete_node(T->right,e,T);
    }
    return NULL;
}

/* 从树的根结点到node进行平衡性调整，不平衡需要再平衡，树各结点的深度信息也需要改变 */
static void adjust(Tree **T,Tree *node)
{
    Tree *tree = *T;
    if(tree->data > node->data)
    {
        adjust(&tree->left,node);
    }
    if(tree->data < node->data)
    {
        adjust(&tree->right,node);
    }
    int len1 = get_depth(tree->left);
    int len2 = get_depth(tree->right);
    if(len1 - len2 == 2)
    {
        // 进行向右的平衡旋转
        right_balance(T);
    }else if(len1 - len2 == -2)
    {
        // 进行向左的平衡旋转
        left_balance(T);
    }else{
        // 不需要再平衡，但是根结点（*T）的深度需要更新，也可能没变
        (*T)->dp = ((len1 > len2)? len1 : len2) + 1;
    }
    return;     
}

/* 从树中删除值为e的结点，结点可能不存在 */
void delete(Tree **T,ElemType e)
{
    Tree *tree = *T;
    // 如果是一颗空树，没有查找的意义
    if(!tree)
    {
        printf("AVL树为空树,没有查找的必要性!\n");
        return;
    }   
    Tree *node;
    if(tree->data == e)    // 删除的结点刚刚好是根结点
    {
        // 如果左右子树不为空，可以找到前驱或者后继
        if(tree->left || tree->right)
        {
            // 查找前驱后继并替代
            node = replace(tree);      // 替代策略
        }else{
            // tree是根结点，并且它没有左子树和右子树
            free(tree);     // 直接释放它
            *T = NULL;      // 并且置空
            printf("AVL树已经被删除干净!\n");
            return;
        }
    }else if(tree->data < e)    // 去左子树继续查找
    {
        node = delete_node(tree->right,e,tree);
    }else if(tree->data > e)    // 去右子树查找
    {
        node = delete_node(tree->left,e,tree);
    }
    // 成功删除一个结点，node为删除结点的前驱
    if(node)
    {
        // 在删除一个结点后，从删除结点的父结点p往上到根结点
        // 平衡性可能发生变化，需要进行调整
        adjust(T,node);
        printf("值为%d的结点被删除,AVL完成调整\n",e);
    }else   // 说明树中没有e的结点
        printf("值为%d的结点不存在\n",e);
    return;
}

/* 往AVL树中插入结点 */
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
        // 尝试在左子树进行插入
        if(!insert(&tree->left,e))
        {
            return false;
        }else{
            int len1 = get_depth(tree->left);
            int len2 = get_depth(tree->right);
            if(len1 - len2 == 2)
            {
                // 进行向右的平衡旋转
                right_balance(T);
            }else{
                // 不需要再平衡，但是根结点（*T）的深度需要更新，因为插入成功了
                (*T)->dp = ((len1 > len2)? len1 : len2) + 1;
            }
            return true;
        }
    }else{
        // 尝试在右子树进行插入
        if(!insert(&tree->right,e))
        {
            return false;
        }else{
            int len1 = get_depth(tree->left);
            int len2 = get_depth(tree->right);
            if(len1 - len2 == -2)
            {
                // 进行向左的平衡旋转
                left_balance(T);
            }else{
                // 不需要再平衡，但是根结点（*T）的深度需要更新，因为插入成功了
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