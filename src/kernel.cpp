#include <stdint.h>
#include <stdbool.h>

#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "mouse.h"
#include "print.h"
#include "cpu.h"
#include "fpu.h"
#include "pci.h"
#include "driver.h"
#include "vga.h"
#include "desktop.h"

extern "C" void kernelMain(void) {
    
    clearScreen();

    GDT gdt;
    gdt.Activate();

    IDT idt(gdt);
    idt.Activate();

    FPU::Init();

    DriverManager drivers;

    PCI pci;
    pci.SelectDrivers(&drivers, &idt);

    Desktop desktop(320, 200, 1);

    Keyboard kbd(&idt);
    drivers.AddDriver(&kbd);
    ConsoleKeyboardHandler console;
    //kbd.SetEventHandler(&console);
    kbd.SetEventHandler(&desktop);

    Mouse mouse(&idt);
    drivers.AddDriver(&mouse);
    mouse.SetEventHandler(&desktop);

    //ConsoleMouseHandler consoleMouse;
    //mouse.SetEventHandler(&consoleMouse);

    VGAGraphicsMode vgaGraphics;
    vgaGraphics.Activate();

    drivers.ActivateAll();
    EnableInterrupts();

    while (true) {
        
        
        desktop.Draw(&vgaGraphics);
        vgaGraphics.PresentVSync();
        
    }

    while (true);

}