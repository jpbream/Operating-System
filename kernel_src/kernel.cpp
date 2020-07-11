#include <stdint.h>
#include <stdbool.h>

#include "gdt.h"
#include "idt.h"
#include "keyboard.h"

extern "C" void kernelMain(void) {
    
    GDT gdt;
    gdt.Activate();

    IDT idt(gdt);
    idt.Activate();

    Keyboard kbd;
    idt.SetHandler(0x21, &kbd);

    EnableInterrupts();

    while (true);

}