#include <stdint.h>
#include <stdbool.h>

#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "mouse.h"
#include "print.h"
#include "cpu.h"

extern "C" bool InitFPU();


extern "C" void kernelMain(void) {
    
    clearScreen();

    if ( !InitFPU() ) {
        printf("You don't have a Floating Point Unit!\n");
        while ( true );
    }

    GDT gdt;
    gdt.Activate();

    IDT idt(gdt);
    idt.Activate();

    Intel_CPU cpu;

    Keyboard kbd;
    idt.SetHandler(0x21, &kbd);

    Mouse mouse;
    idt.SetHandler(0x2C, &mouse);

    EnableInterrupts();

    printf("%s\n", cpu.VendorID());
    printf("%s\n\n", cpu.BrandNameLong());
    for ( int i = 0; i < cpu.NumCacheDescriptors(); ++i ) {
        printf("%s\n", cpu.GetCacheDescriptor(i));
    }

    while (true);

}