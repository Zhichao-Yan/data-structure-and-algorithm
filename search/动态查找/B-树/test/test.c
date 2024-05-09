#include "B.h"

int main()
{
    BTree *T;
    ElemType A[11] = {45,24,53,90,37,50,61,70,100,3,12};
    for(int i = 0; i < 11; ++i)
    {
        insert(&T,A[i]);
    }
    drop(&T,90);
    drop(&T,37);
    drop(&T,3);
    drop(&T,61);
}