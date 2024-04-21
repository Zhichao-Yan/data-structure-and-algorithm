#include "BST.h"

int main()
{
    int A[7] = {45,24,53,47,12,36,90};
    Tree *T = build_tree(A,7);
    show(T);
    delete(&T,45);
    show(T);
    insert(&T,45);
    show(T);
}