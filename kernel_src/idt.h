#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h>
#include "gdt.h"
#include "port.h"

void InitPIC();
extern "C" void EnableInterrupts();
extern "C" void DisableInterrupts();

class IDT {

private:
    uint16_t size;
    uint32_t address;

    struct IDT_Selector {
        uint16_t offset_lo;
        uint16_t selector;
        uint8_t zero_byte;
        uint8_t type_attr;
        uint16_t offset_hi;
    } __attribute__((packed));

    IDT_Selector table[256];

    void CreateSelector(
        uint8_t intNumber,
        uint16_t codeSegOffset,
        void (*handler)(),
        uint8_t privilege,
        uint8_t type
    );

public:
    IDT(GDT& gdt);

} __attribute__((packed));

extern "C" void ActivateIDT(IDT* idt);

#endif