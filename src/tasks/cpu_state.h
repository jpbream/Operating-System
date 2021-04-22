#ifndef _CPU_STATE_H_
#define _CPU_STATE_H_

#include <stdint.h>

struct CPUState {

    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;

    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;

    //uint32_t gs;
    //uint32_t fs;
    //uint32_t es;
    //uint32_t ds;

    uint32_t error;

    // pushed by the processor when an interrupt occurs
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;

} __attribute__((packed));

#endif