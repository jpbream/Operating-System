#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>

#define KERNEL_CODE_ACCESS 0b10011010
#define KERNEL_DATA_ACCESS 0b10010010
#define KERNEL_LIMIT 0x00FFFFFF
#define KERNEL_SEGMENT_FLAGS 0b11110100

class GDT {

private:
    
    class GDT_Selector {

    private:
        uint16_t limit_lo;
        uint16_t base_lo;
        uint8_t base_mid;
        uint8_t access;
        uint8_t flags;
        uint8_t base_hi;

    public:
        GDT_Selector(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

    } __attribute__((packed));

    uint16_t size;
    uint32_t address;

    GDT_Selector null;
    GDT_Selector unused;
    GDT_Selector code;
    GDT_Selector data;

public:

    GDT();
    uint16_t CodeSelector();
    uint16_t DataSelector();

} __attribute__((packed));

extern "C" void ActivateGDT(GDT* gdt);

#endif