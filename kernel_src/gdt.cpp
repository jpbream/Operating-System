#include "gdt.h"

GDT::GDT_Selector::GDT_Selector(uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) 
{
    
    this->base_lo = base & 0xFFFF;
    this->base_mid = (base >> 16) & 0xFF;
    this->base_hi = (base >> 24) & 0xFF;

    this->limit_lo = limit & 0xFFFF;
    this->gran = (limit >> 16) & 0xF;

    this->gran |= gran & 0xF0;
    this->access = access;

}

GDT::GDT() 
    : null(0, 0, 0, 0), code(0, 0xFFFFFFFF, 0x9A, 0xCF), 
    data(0, 0xFFFFFFFF, 0x92, 0xCF)
{
    size = sizeof(GDT) - 1 - sizeof(uint16_t) - sizeof(uint32_t);
    address = (uint32_t)this + sizeof(uint16_t) + sizeof(uint32_t);
}