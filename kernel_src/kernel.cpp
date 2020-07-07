#include <stdint.h>
#include <stdbool.h>

#include "gdt.h"
#include "idt.h"
#include "print.h"

extern "C" void kernelMain(void) {
    
    GDT gdt;
    ActivateGDT(&gdt);

    IDT idt(gdt);
    InitPIC();
    ActivateIDT(&idt);
    EnableInterrupts();

    while (true);

}