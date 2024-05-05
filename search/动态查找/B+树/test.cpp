#include "B+.hpp"

int main()
{
    Record r[16] = {5,8,10,15,16,17,18,19,20,21,22,6,9,7,23,24};
    bp_tree bp(5);
    for(int i = 0; i < 14; ++i)
    {
        bp.insert(&r[i]);
    }
    bp.traverse();
    bp.drop(22);
    bp.traverse();
    bp.drop(15);
    bp.traverse();
    bp.drop(7);
    bp.traverse();

    // bp.drop(15);
    // bp.drop(19);
    // bp.drop(22);
}