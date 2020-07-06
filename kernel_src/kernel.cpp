#include <stdint.h>
#include <stdbool.h>

#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "print.h"

extern "C" {
    GDT gdt;
    IDT idt;
}

extern "C" void kernelMain(void) {

    while (true);

}