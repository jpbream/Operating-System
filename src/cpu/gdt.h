#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>

class GDT {

private:
    
    static constexpr int NUM_SELECTORS = 10;

    struct GDT_Selector {

        uint64_t limit_lo : 16;
        uint64_t base_lo : 16;
        uint64_t base_mid : 8;
        uint64_t access : 8;
        uint64_t limit_mid : 4;
        uint64_t flags : 4;
        uint64_t base_hi : 8;

    } __attribute__((packed));

    void InitSelector(GDT_Selector* selector, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
    uint8_t CreateAccess(uint8_t present, uint8_t privl, uint8_t type, uint8_t exe, uint8_t dirCon, uint8_t rw);
    uint8_t CreateFlags(uint8_t gran, uint8_t size, uint8_t available);

    struct GDT_Pointer {

        uint16_t size;
        uint32_t address;

    } __attribute__((packed));

    GDT_Selector selectors[NUM_SELECTORS] __attribute__((aligned(8)));

public:

    GDT();
    uint16_t CodeSelector();
    uint16_t DataSelector();

    void Activate();

};

#endif