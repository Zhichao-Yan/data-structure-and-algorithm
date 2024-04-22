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
    show(T);
}