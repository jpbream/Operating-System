#include "idt.h"
#include "io.h"
#include "print.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21

#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define INIT 0x11

#define INTERRUPT_PRESENT 0x80
#define INTERRUPT_GATE 0xE

extern "C" void ignore();
extern "C" void irq0();

IDT::IDT() {

    size = sizeof(IDT_Selector) * 256 - 1;
    address = (uint32_t)(this) + sizeof(uint16_t) + sizeof(uint32_t);

    uint16_t codeSegmentOffset = 8;

    for (int i = 0; i < 256; ++i) {
        CreateSelector(i, codeSegmentOffset, &ignore, 0, INTERRUPT_GATE);
    }

    CreateSelector(32, codeSegmentOffset, &irq0, 0, INTERRUPT_GATE);
    CreateSelector(33, codeSegmentOffset, &irq0, 0, INTERRUPT_GATE);

}

void IDT::CreateSelector(
        uint8_t intNumber,
        uint16_t codeSegOffset,
        void (*handler)(),
        uint8_t privilege,
        uint8_t type
    )
{

    table[intNumber].offset_lo = (uint32_t)handler & 0xFFFF;
    table[intNumber].selector = codeSegOffset;
    table[intNumber].zero_byte = 0;
    table[intNumber].type_attr = INTERRUPT_PRESENT | type | ((privilege & 3) << 5);
    table[intNumber].offset_hi = ((uint32_t)handler & 0xFFFF0000) >> 16;

}

extern "C" void InitPIC() {

    outb(PIC1_COMMAND, INIT);
    io_wait();
    outb(PIC2_COMMAND, INIT);
    io_wait();

    outb(PIC1_DATA, 32);
    io_wait();
    outb(PIC2_DATA, 40);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, 1);
    io_wait();
    outb(PIC2_DATA, 1);
    io_wait();

    outb(PIC1_DATA, 0);
    io_wait();
    outb(PIC2_DATA, 0);
    io_wait();

}