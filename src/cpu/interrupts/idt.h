#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h>
#include "gdt.h"
#include "port.h"
#include "interrupt_handler.h"
#include "task_manager.h"
#include "cpu_state.h"

extern "C" void EnableInterrupts();
extern "C" void DisableInterrupts();

class IDT {

    friend CPUState* DelegateToIDT(uint8_t interrupt, CPUState* regs);

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
    TaskManager* taskManager;

    void CreateSelector(
        uint8_t intNumber,
        uint16_t codeSegOffset,
        void (*handler)(),
        uint8_t privilege,
        uint8_t type
    );

    CPUState* HandleInterrupt(uint8_t interruptNumber, CPUState* regs);

public:
    IDT(GDT& gdt, TaskManager& taskManager);

    void Activate();
    void SetHandler(uint8_t interruptNumber, InterruptHandler* handler);

};

#endif