#include "gdt.h"
#include "print.h"

// options for the 'access' field
#define PRESENT (1 << 7)
#define NOT_PRESENT 0

#define PRIV_KERNEL 0
#define PRIV_USER (0b11 << 5)

#define TYPE_CODE (1 << 4)
#define TYPE_DATA (1 << 4)
#define TYPE_SYSTEM 0

#define EXECUTABLE (1 << 3)
#define NOT_EXECUTABLE 0

#define DIRECTION_GROW_UP 0
#define DIRECTION_GROW_DOWN (1 << 2)
#define CONFORM_LOOSE (1 << 2)
#define CONFORM_TIGHT 0

#define CODE_READ_ENABLE (1 << 1)
#define CODE_READ_DISABLE 0
#define DATA_WRITE_ENABLE (1 << 1)
#define DATA_WRITE_DISABLE 0

// options for the 'flags' field
#define GRAN_BYTE 0
#define GRAN_PAGE (1 << 3)

#define SIZE_32BIT (1 << 2)
#define SIZE_16BIT 0

#define SOFTWARE_AVAILABLE 1
#define SOFTWARE_UNAVAILABLE 0

extern "C" void ActivateGDT(void* gdt);

GDT::GDT() 
{
    uint8_t flags = CreateFlags(GRAN_PAGE, SIZE_32BIT, SOFTWARE_AVAILABLE);
    uint8_t kernelCodeAccess 
        = CreateAccess(PRESENT, PRIV_KERNEL, TYPE_CODE, EXECUTABLE, CONFORM_TIGHT, CODE_READ_ENABLE);
    uint8_t kernelDataAccess
        = CreateAccess(PRESENT, PRIV_KERNEL, TYPE_DATA, NOT_EXECUTABLE, DIRECTION_GROW_UP, DATA_WRITE_ENABLE);

    uint8_t userCodeAccess
        = CreateAccess(PRESENT, PRIV_USER, TYPE_CODE, EXECUTABLE, CONFORM_TIGHT, CODE_READ_ENABLE);
    uint8_t userDataAccess
        = CreateAccess(PRESENT, PRIV_USER, TYPE_DATA, NOT_EXECUTABLE, DIRECTION_GROW_UP, DATA_WRITE_ENABLE);

    InitSelector(&selectors[0], 0, 0, 0, 0); // null selector
    InitSelector(&selectors[1], 0, 0, 0, 0); // unused selector

    InitSelector(&selectors[2], 0, 0xFFFFF, kernelCodeAccess, flags); // kernel code
    InitSelector(&selectors[3], 0, 0xFFFFF, kernelDataAccess, flags); // kernel data

    // zero the rest
    for (int i = 4; i < NUM_SELECTORS; ++i)
        InitSelector(&selectors[i], 0, 0, 0, 0); // null selector

}

void GDT::InitSelector(GDT_Selector* selector, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    selector->base_lo = base & 0xFFFF;
    selector->base_mid = (base >> 16) & 0xFF;
    selector->base_hi = (base >> 24) & 0xFF;

    selector->limit_lo = limit & 0xFFFF;
    selector->limit_mid = (limit >> 16) & 0xF;

    selector->access = access;
    selector->flags = flags;
}

uint8_t GDT::CreateAccess(uint8_t present, uint8_t privl, uint8_t type, uint8_t exe, uint8_t dirCon, uint8_t rw)
{
    return present | privl | type | exe | dirCon | rw;
}

uint8_t GDT::CreateFlags(uint8_t gran, uint8_t size, uint8_t available)
{
    return gran | size | available;
}

uint16_t GDT::CodeSelector() {

    return (uint8_t*)&selectors[2] - (uint8_t*)&selectors;

}
uint16_t GDT::DataSelector() {

    return (uint8_t*)&selectors[3] - (uint8_t*)&selectors;

}

void GDT::Activate()
{
    GDT_Pointer gdtPtr;
    gdtPtr.size = sizeof(GDT_Selector) * NUM_SELECTORS - 1;
    gdtPtr.address = (uint32_t)&selectors[0];

    ActivateGDT(&gdtPtr);
}