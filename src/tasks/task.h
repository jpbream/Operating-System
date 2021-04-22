#ifndef _TASK_H_
#define _TASK_H_

#include "gdt.h"
#include "cpu_state.h"
#include <stdint.h>

class TaskManager;

class Task {

    friend class TaskManager;
private:
    uint8_t stack[4096];
    CPUState* cpuState;

public:
    Task(GDT* gdt, void (*entryPoint)());

};

#endif