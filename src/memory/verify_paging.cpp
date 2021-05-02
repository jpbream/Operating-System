#include <stdint.h>

struct Address
{
    uint32_t offset : 12;
    uint32_t table : 10;
    uint32_t dir : 10;
};

// ensures a page table was set up correctly by ensuring every address
// maps to itself
bool VerifyPaging(uint32_t cr3)
{
    for (uint32_t i = 0; i < 0x1FFFFFFF; ++i) {

        Address addr = *(Address*)&i;
        
        uint32_t fromPageDir = *(uint32_t*)(cr3 + addr.dir * 4);
        fromPageDir >>= 12;

        uint32_t fromTable = *(uint32_t*)((fromPageDir << 12) | (addr.table * 4));
        fromTable >>= 12;

        uint32_t final = (fromTable << 12) | (addr.offset);


        if (final != i) {
            return false;
        }
    }

    return true;
}