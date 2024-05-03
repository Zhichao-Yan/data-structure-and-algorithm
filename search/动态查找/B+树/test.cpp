#include "B+.hpp"

int main()
{
    Record r[14] = {5,8,10,15,16,17,18,19,20,21,22,6,9,7};
    bp_tree bp(5);
    for(int i = 0; i < 14; ++i)
    {
        bp.insert(&r[i]);
    }
    bp.drop(22);
    
}