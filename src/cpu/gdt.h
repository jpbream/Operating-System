#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>

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

    struct GDT_Pointer {

        uint16_t size;
        uint32_t address;

    } __attribute__((packed));

    GDT_Selector null;
    GDT_Selector unused;
    GDT_Selector code;
    GDT_Selector data;

public:

    GDT();
    uint16_t CodeSelector();
    uint16_t DataSelector();

    void Activate();

};

#endif