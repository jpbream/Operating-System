#include "task.h"

Task::Task(GDT* gdt, void (*entry)())
{
    cpuState = (CPUState*)(stack + 4096 - sizeof(CPUState));

    // set the cpu state to all zero
    for (int i = 0; i < sizeof(CPUState); ++i) {
        *(uint8_t*)cpuState = 0;
    }

    cpuState->eip = (uint32_t)entry;
    cpuState->eflags = 0x202;
    cpuState->cs = gdt->CodeSelector();
}