#include "gdt.h"

extern "C" constexpr uint16_t GDT_SIZE = sizeof(GDT) - 1;

GDT::GDT_Selector::GDT_Selector(uint32_t base, uint32_t limit, uint16_t flag) 
{
    
    descriptor = 0;

    // high 32 bits
    descriptor |= (base & 0xFF000000);
    descriptor |= ((flag & 0x0F00) << 12);
    descriptor |= (limit & 0x000F0000);
    descriptor |= ((flag & 0x00FF) << 8);
    descriptor |= ((base & 0x00FF0000) >> 16);
    descriptor <<= 32;

    // low 32 bits
    descriptor |= ((base & 0x0000FFFF) << 16);
    descriptor |= (limit & 0x0000FFFF);

}

GDT::GDT() 
    : null(0, 0, 0), code(0, 0x000FFFFF, CODE_ACCESS | CODE_FLAGS), 
    data(0, 0x000FFFFF, DATA_ACCESS | DATA_FLAGS)
{
}