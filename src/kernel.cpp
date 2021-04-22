#include <stdint.h>
#include <stdbool.h>

#include "gdt.h"
#include "idt.h"
#include "ps2_keyboard.h"
#include "ps2_mouse.h"
#include "print.h"
#include "cpu.h"
#include "fpu.h"
#include "pci.h"
#include "driver.h"
#include "vga.h"
#include "desktop.h"
#include "console.h"
#include "window.h"
#include "task_manager.h"
#include "task.h"

void taskA() {
    while (true) {
        int num = 100000000;
        while (num--);
        printf("A");
    }
}

void taskB() {
    while (true) {
        int num = 100000000;
        while (num--);
        printf("B");
    }
}

extern "C" void kernelMain(void) {
    
    clearScreen();

    GDT gdt;
    gdt.Activate();

    TaskManager taskManager;
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);

    IDT idt(gdt, taskManager);
    idt.Activate();

    FPU::Init();

    DriverManager drivers;

    PCI pci;
    pci.SelectDrivers(&drivers, &idt);

    // Desktop desktop(320, 200, 1);
    // Window window(&desktop, 50, 25, 50, 50, 2);
    // Window window2(&desktop, 200, 25, 50, 50, 3);
    // desktop.AddChild(&window);
    // desktop.AddChild(&window2);
    Console console;

    PS2Keyboard kbd(&idt);
    drivers.AddDriver(&kbd);
    //kbd.SetEventHandler(&desktop);
    kbd.SetEventHandler(&console);

    PS2Mouse mouse(&idt);
    drivers.AddDriver(&mouse);
    //mouse.SetEventHandler(&desktop);
    mouse.SetEventHandler(&console);

    //VGAGraphicsMode vgaGraphics;
    //vgaGraphics.Activate();

    VGATextMode vgaText;
    vgaText.Activate();

    drivers.ActivateAll();



    EnableInterrupts();

    while (true) {
        //desktop.Draw(&vgaGraphics);
        //vgaGraphics.PresentVSync();
    }

    while (true);

}