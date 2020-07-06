#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>

#define CODE_ACCESS 0b10011010
#define CODE_FLAGS ((0b0100) << 8)

#define DATA_ACCESS 0b10010010
#define DATA_FLAGS ((0b0100) << 8)

class GDT {

public:
    
    class GDT_Selector {

    public:
        uint16_t limit_lo;
        uint16_t base_lo;
        uint8_t base_mid;
        uint8_t access;
        uint8_t gran;
        uint8_t base_hi;

    public:
        GDT_Selector(uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

    } __attribute__((packed));

    uint16_t size;
    uint32_t address;

    GDT_Selector null;
    GDT_Selector code;
    GDT_Selector data;

public:

    GDT();

} __attribute__((packed));


#endif