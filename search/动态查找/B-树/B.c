#include "B.h"

/* 使用二分查找在结点中查找值key的位置范围 */
static int binary_search(ElemType key[], int size, ElemType target) 
{
    int left = 1,right = size;
    while (left <= right) 
    {
        int mid = (left + right) / 2;
        if (key[mid] == target) 
        {
            return mid;  // 找到目标值，返回其索引
        }else if(key[mid] < target) 
        {
            left = mid + 1;  // 目标值在右半区间，更新左边界
        }else 
        {
            right = mid - 1;  // 目标值在左半区间，更新右边界
        }
    }
    // right < left
    return right;  // 始终返回right 
}

/* 在结点中查找值key的位置范围 */
static int search_node(BTree *p,ElemType key)
{
    int i;
    for(i = 1; i <= p->key_num; ++i)
    {
        if(key < p->key[i])
            break;

    }
    return i - 1;
}

/* 在B树中查找值key */
Result search(BTree *T,ElemType key)
{
    BTree *p = T,*f = NULL;
    int i = 0;
    while(p)
    {
        i = binary_search(p->key,p->key_num,key);
        //i = search_node(p,key);
        if(i >0 && p->key[i] == key)
        {
            // 查找成功，key在p指向的结点中i位置，即p->key[i] == key
            Result r = {p,i,yes};
            return r;
        }else{
            f = p;
            p = p->ptr[i];
        }
    }
    // 查找是吧，f结点中没有key的值，其范围位于(p->key[i],p->key[i + 1])之间
    Result r = {f,i,no};
    return r;
}

/* B树为空时或者根结点被裂解时，插入根结点,只有一个键值*/
static void insert_root(BTree **T,BTree *p,ElemType key,BTree *q)
{
    *T = (struct node*)malloc(sizeof(struct node));
    (*T)->key_num = 1;
    (*T)->ptr[0] = p;
    (*T)->key[1] = key;
    (*T)->ptr[1] = q;
    (*T)->parent = NULL;
    // 设置p的父结点为*T
    if(p)
    {
        p->parent = *T;
    }
    // 设置q的父结点为*T
    if(q)
    {
        q->parent = *T;
    }
    return;
}



/* 在B树中插上值key */
void insert(BTree **T,ElemType key)
{
    // B树为空
    if(*T == NULL)
    {
        insert_root(T,NULL,key,NULL);
        return;
    }
    // B树不为空
    BTree *tree = *T;
    // 在B树中查找key，返回结果r
    Result r = search(tree,key);
    if(r.tag == yes)
    {
        printf("值%d在B树中\n",key);
    }else{
        // 查找不成功
        // key其范围位于(p->key[i],p->key[i + 1])之间
        // 插入在第i和第i+1之间，即变成第i+1位置的数据
        struct node *p = r.ptr;
        int i = r.i + 1;
        insert_node(T,p,i,key);
    }
    return;
}

/* 将key和ptr插入到结点信息中的第i个位置，原来从i到key_num数据后移一位 */
static void insert_info(BTree *p,int i,ElemType key,struct node *ptr)
{
    for(int j = p->key_num; j >= i; --j)
    {
        p->key[j + 1] = p->key[j];
        p->ptr[j + 1] = p->ptr[j];
    }
    p->key[i] = key;
    p->ptr[i] = ptr;
    ++p->key_num;   // 自增
    return;
}

/* 裂解指针p，返回ptr */
static struct node *split(struct node *p,int k)
{
    struct node *ptr = (struct node*)malloc(sizeof(struct node));
    ptr->key_num = m - k;  // m - k
    ptr->ptr[0] = p->ptr[k];
    for(int j = 1; j <= ptr->key_num; ++j)
    {
        ptr->key[j] = p->key[k + j];
        p->key[k + j] = 0;

        ptr->ptr[j] = p->ptr[k + j];
        p->ptr[k + j] = NULL;
    }
    p->key_num = k - 1;
    return ptr;
}

/* 在p指向的结点中插入第i的位置插入值key，如果引起结点过大，则沿着双亲链进行裂解 */
static void insert_node(BTree **T,BTree *p,int i,ElemType key)
{
    struct node *ptr = NULL;
    while(p)
    {
        // 先进行插入
        insert_info(p,i,key,ptr);
        // 结点没有满：结点子树的个数<=m,完成插入动作
        if(p->key_num < m)
        {
            break;
        }else if( p->key_num == m)  // 值为m个，子树已经为m+1个，需要进行分裂
        {
            int k = (int)ceil((double)m/2);  // 去m/2的浮点数的天花板值
            key = p->key[k];        // 准备插入父结点的值key
            ptr = split(p,k);       // 裂解结点p，返回指向结点的ptr
            struct node *father = p->parent;
            // 如果父结点存在
            if(father)
            {
                // 设置ptr的父结点
                ptr->parent = father;
                p = father;
                // 计算在p中key的实际插入位置
                // key实际在p中不存在，返回的值是x,key属于(x,x+1)
                // i = x + 1是实际需要的插入位置
                i = binary_search(p->key,p->key_num,key) + 1;
            }else{
                // 到了顶，生成新的树根结点，p和ptr作为左右子树
                insert_root(T,p,key,ptr);
                break;
            }
        }
    }
    return;
}

/* 从结点p中删除第i个位置的键值Ki和指针ptr,后面的值往前移动*/
static void delete_info(BTree *p,int i)
{
    for(int j = i + 1; j <= p->key_num; ++j)
    {
        p->key[j - 1] = p->key[j];
        p->ptr[j - 1] = p->ptr[j];
    }
    --p->key_num;
    return;
}

/* 从B树中删除值key */
void drop(BTree **T,ElemType key)
{   
    // B树为空，直接返回
    if(*T == NULL)
        return;
    BTree *tree;
    Result r = search(tree,key);
    if(r.tag == no)
    {
        printf("键值%d在B树中不存在\n",key);
        return;
    }else{
        // 查找成功，在p指向的结点中第i个关键字
        struct node *p = r.ptr;
        int i = r.i;
        struct node *q = p->ptr[i];
        // 说明p结点不是最下层的非终端结点，实行替代策略
        // 使用p->ptr[i]子树中最小值替代
        if(q)
        {
            while(q)
            {
                p = q;
                q = q->ptr[0]; 
            }
            // 替代
            r.ptr->key[r.i] = p->key[1];
            i = 1;
        }
        // 获得被删除的关键字，将其保留做二分查找用
        ElemType key = p->key[i];
        delete_info(p,i);   // 直接删除从p指向的结点中删除第i位置的键值
        // 此处p指向最下层的非终端结点，i为其中被删除的键值
        int k = (int)ceil((double)m/2);  // 去m/2的浮点数的天花板值
        // 删除一个键后不用调整，满足B树对结点的最小子树数量要求

        while(p)
        {
            // 被删除之后，结点的关键字个数
            if(p->key_num >= k - 1)
            {
                break;
            }else if(p->key_num < k -1) // 被删除之后不满足结点关键字和子树数量要求
            {
                struct node *father = p->parent;
                if(father)
                {
                    // 在father结点中二分查找p所在的范围，得到p = father->ptr[loc]
                    int loc = binary_search(father->key,father->key_num,key); 
                    // 求得p的左右兄弟结点
                    struct node *left_sibling,*right_sibling;
                    if(loc != 0)
                    {
                        left_sibling = father->ptr[loc - 1];
                    }else
                        left_sibling = NULL;
                    if(loc != father->key_num)
                    {
                        right_sibling = father->ptr[loc + 1];
                    }else
                        right_sibling = NULL;

                    // 因为从p中删除后键值数量不满足要求，所以尝试从左或者右兄弟借出
                    if((right_sibling && right_sibling->key_num >= k) || \
                        (left_sibling && left_sibling->key_num >= k))
                    {
                        if(right_sibling && right_sibling->key_num >= k)    // 从右兄弟借最小值
                        {
                            // 将双亲结点中的key[loc + 1]值插入p中尾部位置
                            // 因为是key[loc + 1]的值肯定大于所有p中的现有值
                            insert_info(p,p->key_num + 1,father->key[loc + 1],right_sibling->ptr[1]);    
                            father->key[loc + 1] = right_sibling->key[1];   // 将右兄弟结点中的最小值移到双亲结点中该位置
                            delete_info(right_sibling,1);   // 从右兄弟中删除最小值

                        }else{      // 从左兄弟借最大值
                            // 将父结点中loc位置的该值father->key[loc]
                            // 插入loc子树结点p中位置1处
                            // 因为father->key[loc]肯定小于p中所有值
                            insert_info(p,1,father->key[loc],left_sibling->ptr[left_sibling->key_num]); 
                            father->key[loc] = left_sibling->key[left_sibling->key_num]; // 将左兄弟结点中的最大值移到双亲结点中该位置
                            delete_info(left_sibling,left_sibling->key_num);   // 从左兄弟中删除最大值
                        }
                        // 父结点关键字没有减少，p和左右兄弟结点也都满足关键字和子树数量要求，直接退出循环
                        break;
                    }else{
                        if(right_sibling && right_sibling->key_num == k - 1)  // 右子树没办法借出关键字，考虑合并
                        {
                            // 插入在p结点信息的末尾p->key_num + 1处
                            insert_info(p,p->key_num + 1,father->key[loc + 1],right_sibling->ptr[0]);
                            // 把right_sibling中信息插入到p中p->key_num + 1处
                            for(int j = 1; j <= right_sibling->key_num; ++j)
                            {
                                insert_info(p,p->key_num + 1,right_sibling->key[j],right_sibling->ptr[j]);
                            }
                            // 删除右兄弟结点
                            free(right_sibling);
                            key = father->key[loc + 1];     // 获得被删除的关键字，将其保留做二分查找用
                            // 从父结点中删除loc+1位置的关键字
                            delete_info(father,loc + 1);
                        }else if(left_sibling && left_sibling->key_num == k - 1)    // 左子树没办法借出关键字，考虑合并
                        {
                            // 插入left_sibling的末尾+1处
                            insert_info(left_sibling,left_sibling->key_num + 1,father->key[loc],p->ptr[0]);
                            // 将p中关键字和子树指针插入left_sibling的末尾+1处
                            for(int j = 1; j <= p->key_num; ++j)
                            {
                                insert_info(left_sibling,left_sibling->key_num + 1,p->key[j],p->ptr[j]);
                            }
                            free(p);    // 释放p结点
                            key = father->key[loc];     // 获得被删除的关键字，将其保留做二分查找用
                            delete_info(father,loc);    // 从父结点中删除loc处关键字
                        }
                        p = father; // p开始指向父结点，因为父结点删除了关键字，可能需要做类似处理
                    }
                }else{
                    // 没有父结点，说明是根结点
                    // 根结点最少得一个关键字，如果没有，则可以删除该根结点了
                    if(p->key_num < 1)
                    {
                        *T = p->ptr[0]; // 更换根结点指向p->ptr[0]
                        free(p);        // 释放原来的根结点
                        p = NULL;
                    }else
                        break;      // 否则可以直接退出，不再向上操作
                }
            }
        }
    }
    return;
}


void print_node(BTree *p)
{
    for(int i = 1; i <= p->key_num; ++i)
    {
        printf("%d ",p->key[i]);
    }
    printf("\n");
    return;
}