#include <stdlib.h>
#include <math.h>

typedef char ElemType;
typedef struct node{
    ElemType e;
    struct node *left;
    struct node *right;
}STree;


STree* build_tree(ElemType *keys,float *sw,int low,int high)
{
    /* 查找左右权重和相差绝对值最小的节点 */
    int i = low;
    float min = fabs(sw[high] + sw[low - 1] - sw[low] - sw[low - 1]);
    for(int j = low + 1; j <= high; ++j)
    {
        if(fabs(sw[high] + sw[low - 1] - sw[j] - sw[j - 1]) < min)
        {
            i = j;
            min = fabs(sw[high] + sw[low - 1] - sw[j] - sw[j - 1]);
        }
    }
    STree *T = (STree*)malloc(sizeof(struct node));
    T->e = keys[i - 1];

    /* 构建左子树 */
    if(i == low)
    {
        T->left = NULL;
    }else{
        T->left = build_tree(keys,sw,low,i - 1);
    }
    /* 构建右子树 */
    if(i == high)
    {
        T->right = NULL;
    }else{
        T->right  = build_tree(keys,sw,i + 1,high);
    }
    return T;
}

STree * build_search_tree(ElemType *keys,float *w,int size)
{
    if(size == 0)
        return NULL;
    /* 求累计权重表 */
    float *sw =(float*)mallo((size + 1) * sizeof(float));
    sw[0] = 0;
    for(int i = 1; i <= size; ++i)
    {
        sw[i] = sw[i - 1] + w[i - 1];
    }
    /* 正式构建近似最优查找树 */
    STree *T = build_tree(keys,sw,1,size);
    return T;
}

int main()
{
    // 有序表
    ElemType keys[9] = {'A','B','C','D','E','F','G','H','I'};
    // 有序表对应权重表
    float w[9] = {1,1,2,5,3,4,4,3,5};
    STree *T = build_search_tree(keys,w,9);
    
}