#include "idt.h"
#include "port.h"
#include "print.h"
#include "interrupt_list.h"
#include "exceptions.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21

#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define PIC_INIT 0x11
#define PIC_ACKNOWLEDGE 0x20

#define INTERRUPT_PRESENT 0x80
#define INTERRUPT_GATE 0xE

extern "C" constexpr uint16_t HARDWARE_OFFSET = 0x20;

IDT::IDT(GDT& gdt) 
{

    size = sizeof(IDT_Selector) * 256 - 1;
    address = (uint32_t)(this) + sizeof(uint16_t) + sizeof(uint32_t);

    uint16_t codeSegmentOffset = gdt.CodeSelector();

    for (int i = 0; i < 256; ++i) {
        CreateSelector(i, codeSegmentOffset, &IgnoreInterrupt, 0, INTERRUPT_GATE);
    }

    CreateSelector(0x00, codeSegmentOffset, &HandleException0x00, 0, INTERRUPT_GATE);
    CreateSelector(0x01, codeSegmentOffset, &HandleException0x01, 0, INTERRUPT_GATE);
    CreateSelector(0x02, codeSegmentOffset, &HandleException0x02, 0, INTERRUPT_GATE);
    CreateSelector(0x03, codeSegmentOffset, &HandleException0x03, 0, INTERRUPT_GATE);
    CreateSelector(0x04, codeSegmentOffset, &HandleException0x04, 0, INTERRUPT_GATE);
    CreateSelector(0x05, codeSegmentOffset, &HandleException0x05, 0, INTERRUPT_GATE);
    CreateSelector(0x06, codeSegmentOffset, &HandleException0x06, 0, INTERRUPT_GATE);
    CreateSelector(0x07, codeSegmentOffset, &HandleException0x07, 0, INTERRUPT_GATE);
    CreateSelector(0x08, codeSegmentOffset, &HandleException0x08, 0, INTERRUPT_GATE);
    CreateSelector(0x09, codeSegmentOffset, &HandleException0x09, 0, INTERRUPT_GATE);
    CreateSelector(0x0A, codeSegmentOffset, &HandleException0x0A, 0, INTERRUPT_GATE);
    CreateSelector(0x0B, codeSegmentOffset, &HandleException0x0B, 0, INTERRUPT_GATE);
    CreateSelector(0x0C, codeSegmentOffset, &HandleException0x0C, 0, INTERRUPT_GATE);
    CreateSelector(0x0D, codeSegmentOffset, &HandleException0x0D, 0, INTERRUPT_GATE);
    CreateSelector(0x0E, codeSegmentOffset, &HandleException0x0E, 0, INTERRUPT_GATE);
    CreateSelector(0x0F, codeSegmentOffset, &HandleException0x0F, 0, INTERRUPT_GATE);
    CreateSelector(0x10, codeSegmentOffset, &HandleException0x10, 0, INTERRUPT_GATE);
    CreateSelector(0x11, codeSegmentOffset, &HandleException0x11, 0, INTERRUPT_GATE);
    CreateSelector(0x12, codeSegmentOffset, &HandleException0x12, 0, INTERRUPT_GATE);
    CreateSelector(0x13, codeSegmentOffset, &HandleException0x13, 0, INTERRUPT_GATE);
    CreateSelector(0x14, codeSegmentOffset, &HandleException0x14, 0, INTERRUPT_GATE);
    CreateSelector(0x15, codeSegmentOffset, &HandleException0x15, 0, INTERRUPT_GATE);
    CreateSelector(0x16, codeSegmentOffset, &HandleException0x16, 0, INTERRUPT_GATE);
    CreateSelector(0x17, codeSegmentOffset, &HandleException0x17, 0, INTERRUPT_GATE);
    CreateSelector(0x18, codeSegmentOffset, &HandleException0x18, 0, INTERRUPT_GATE);
    CreateSelector(0x19, codeSegmentOffset, &HandleException0x19, 0, INTERRUPT_GATE);
    CreateSelector(0x1A, codeSegmentOffset, &HandleException0x1A, 0, INTERRUPT_GATE);
    CreateSelector(0x1B, codeSegmentOffset, &HandleException0x1B, 0, INTERRUPT_GATE);
    CreateSelector(0x1C, codeSegmentOffset, &HandleException0x1C, 0, INTERRUPT_GATE);
    CreateSelector(0x1D, codeSegmentOffset, &HandleException0x1D, 0, INTERRUPT_GATE);
    CreateSelector(0x1E, codeSegmentOffset, &HandleException0x1E, 0, INTERRUPT_GATE);
    CreateSelector(0x1F, codeSegmentOffset, &HandleException0x1F, 0, INTERRUPT_GATE);

    CreateSelector(HARDWARE_OFFSET + 0x00, codeSegmentOffset, &HandleInterrupt0x00, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x01, codeSegmentOffset, &HandleInterrupt0x01, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x02, codeSegmentOffset, &HandleInterrupt0x02, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x03, codeSegmentOffset, &HandleInterrupt0x03, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x04, codeSegmentOffset, &HandleInterrupt0x04, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x05, codeSegmentOffset, &HandleInterrupt0x05, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x06, codeSegmentOffset, &HandleInterrupt0x06, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x07, codeSegmentOffset, &HandleInterrupt0x07, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x08, codeSegmentOffset, &HandleInterrupt0x08, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x09, codeSegmentOffset, &HandleInterrupt0x09, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x0A, codeSegmentOffset, &HandleInterrupt0x0A, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x0B, codeSegmentOffset, &HandleInterrupt0x0B, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x0C, codeSegmentOffset, &HandleInterrupt0x0C, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x0D, codeSegmentOffset, &HandleInterrupt0x0D, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x0E, codeSegmentOffset, &HandleInterrupt0x0E, 0, INTERRUPT_GATE);
    CreateSelector(HARDWARE_OFFSET + 0x0F, codeSegmentOffset, &HandleInterrupt0x0F, 0, INTERRUPT_GATE);

}

extern "C" void InterruptCallback(uint8_t interrupt) {
    //printf("INTERRUPT ");
    
    if (interrupt != 0) {
        printf("Tick ");
    }



    if (interrupt >= HARDWARE_OFFSET && interrupt < HARDWARE_OFFSET + 16) {

        Write8Slow(PIC1_COMMAND, PIC_ACKNOWLEDGE);

        if (interrupt >= HARDWARE_OFFSET + 8) {

            Write8Slow(PIC2_COMMAND, PIC_ACKNOWLEDGE);
        }
    }
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

void InitPIC() {

    Port picMasterCommand(PIC1_COMMAND);
    Port picSlaveCommand(PIC2_COMMAND);
    Port picMasterData(PIC1_DATA);
    Port picSlaveData(PIC2_DATA);

    picMasterCommand.Write8Slow(PIC_INIT);
    picSlaveCommand.Write8Slow(PIC_INIT);

    picMasterData.Write8Slow(HARDWARE_OFFSET);
    picSlaveData.Write8Slow(HARDWARE_OFFSET + 8);

    picMasterData.Write8Slow(4);
    picSlaveData.Write8Slow(2);

    picMasterData.Write8Slow(1);
    picSlaveData.Write8Slow(1);

    picMasterData.Write8Slow(0);
    picSlaveData.Write8Slow(0);

}