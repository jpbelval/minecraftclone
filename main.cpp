#include <iostream>
#include  "blockinfo.h"

int main()
{
    BlockInfo blockInfo1(BTYPE_DIRT_, "Terre");
    blockInfo1.SetDurability(52);
    blockInfo1.Show();
}