#include "gdt.h"
#include "print.h"

#define KERNEL_CODE_ACCESS 0b10011010
#define KERNEL_DATA_ACCESS 0b10010010
#define KERNEL_LIMIT 0x00FFFFFF
#define KERNEL_SEGMENT_FLAGS 0b11110100

extern "C" void ActivateGDT(void* gdt);

GDT::GDT_Selector::GDT_Selector(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) 
{
    
    this->base_lo = base & 0xFFFF;
    this->base_mid = (base >> 16) & 0xFF;
    this->base_hi = (base >> 24) & 0xFF;

    this->limit_lo = limit & 0xFFFF;
    this->flags = (limit >> 16) & 0xF;

    this->flags |= flags & 0xF0;
    this->access = access;

}

GDT::GDT() 
    : null(0, 0, 0, 0), unused(0, 0, 0, 0),
    code(0, KERNEL_LIMIT, KERNEL_CODE_ACCESS, KERNEL_SEGMENT_FLAGS), 
    data(0, KERNEL_LIMIT, KERNEL_DATA_ACCESS, KERNEL_SEGMENT_FLAGS)
{
}

uint16_t GDT::CodeSelector() {

    return (uint8_t*)&code - (uint8_t*)&null;

}
uint16_t GDT::DataSelector() {

    return (uint8_t*)&data - (uint8_t*)&null;

}

void GDT::Activate()
{
    GDT_Pointer gdtPtr;
    gdtPtr.size = sizeof(GDT_Selector) * 4 - 1;
    gdtPtr.address = (uint32_t)&null;

    ActivateGDT(&gdtPtr);
}