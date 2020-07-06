#include "print.h"
#include "io.h"

extern "C" void irq0_handler() {
    printf("Interrupt");
    outb(0xA0, 0x20);
    outb(0x20, 0x20); // EOI signal
}

extern "C" void irq_ignore() {
    
    outb(0x20, 0x20);
}