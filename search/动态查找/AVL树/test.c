#include "AVL.h"


int main()
{
    Tree *T;
    int A[10] = {3,2,1,4,5,6,7,10,9,8};
    for(int i = 0; i < 10; ++i)
    {
        if(!insert(&T,A[i]))
        {
            printf("%d插入失败\n",A[i]);
        }
    }
    // show(T);
    // Tree *node = search(T,10);
    // if(node)
    //     printf("成功查找到%d的结点\n",node->data);
    // else
    //     printf("没有找到结点\n");
    // node = search(T,100);
    // if(node)
    //     printf("成功查找到%d的结点\n",node->data);
    // else
    //     printf("没有找到结点\n");
    // delete(&T,100);
    // delete(&T,4);
    delete1(&T,100);
    delete1(&T,4);
    show(T);
}