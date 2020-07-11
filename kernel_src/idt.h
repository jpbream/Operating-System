#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h>
#include "gdt.h"
#include "port.h"
#include "interrupt_handler.h"

extern "C" void EnableInterrupts();
extern "C" void DisableInterrupts();

class IDT {

    friend void DelegateToIDT(uint8_t interrupt);

private:

    struct IDT_Pointer {
        uint16_t size;
        uint32_t address;
    } __attribute__((packed));

    void InitPIC();

    struct IDT_Selector {
        uint16_t offset_lo;
        uint16_t selector;
        uint8_t zero_byte;
        uint8_t type_attr;
        uint16_t offset_hi;
    } __attribute__((packed));

    IDT_Selector table[256];

    Port picMasterCommand;
    Port picSlaveCommand;
    Port picMasterData;
    Port picSlaveData;

    DefaultExceptionHandler defExceptionHandler;
    DefaultInterruptHandler defInterruptHandler;
    InterruptHandler* handlers[256];

    void CreateSelector(
        uint8_t intNumber,
        uint16_t codeSegOffset,
        void (*handler)(),
        uint8_t privilege,
        uint8_t type
    );

    void HandleInterrupt(uint8_t interruptNumber);

public:
    IDT(GDT& gdt);

    void Activate();
    void SetHandler(uint8_t interruptNumber, InterruptHandler* handler);

};

#endif