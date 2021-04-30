#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "gdt.h"
#include "idt.h"
#include "ps2_keyboard.h"
#include "ps2_mouse.h"
#include "print.h"
#include "cpu.h"
#include "fpu.h"
#include "pci.h"
#include "driver.h"
#include "vga_graphics.h"
#include "desktop.h"
#include "console.h"
#include "window.h"
#include "task_manager.h"
#include "task.h"
#include "multiboot.h"
#include "graphics_context.h"
#include "sse.h"
#include "memory.h"
#include "page_manager.h"
#include "memory_heap.h"
#include "linked_memory_heap.h"

#define TEXT_MODE true

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

MemoryHeap* kernelHeap;

void* operator new(size_t size)
{
    return kernelHeap->Allocate(size);
}

void operator delete(void* ptr)
{
    kernelHeap->Free(ptr);
}

void operator delete(void* ptr, unsigned int)
{
    kernelHeap->Free(ptr);
}

PageManager pm;


extern "C" void kernelMain(multiboot_info_t* info, uint32_t magicNumber) {
    
    clearScreen();

    GDT gdt;
    gdt.Activate();

    TaskManager taskManager;
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    //taskManager.AddTask(&task1);
    //taskManager.AddTask(&task2);

    IDT idt(gdt, taskManager);
    idt.Activate();

    FPU::Init();
    SSE::Init();

    // start the heap at 10 Mb
    size_t heap = 10 * 1024 * 1024;                     // padding
    uint32_t memUpper = info->mem_upper * 1024 - heap - 10 * 1024;

    printf("Heap Size: %d Mb\n", memUpper / 1024 / 1024);

    
    pm.Activate();

    LinkedMemoryHeap linkedHeap(heap, memUpper);
    kernelHeap = &linkedHeap;

    DriverManager drivers;

    PCI pci;
    //pci.SelectDrivers(&drivers, &idt);

    Desktop desktop(
        info->framebuffer_width > 400 ? info->framebuffer_width : 320, 
        info->framebuffer_width > 400 ? info->framebuffer_height : 200, 
        COLOR(0, 0, 255));

    Window window(&desktop, 50, 25, 50, 50, COLOR(0, 255, 0));
    Window window2(&desktop, 200, 25, 50, 50, COLOR(0, 255, 0));
    desktop.AddChild(&window);
    desktop.AddChild(&window2);

    Console console;

    PS2Keyboard kbd(&idt);
    drivers.AddDriver(&kbd);

    PS2Mouse mouse(&idt);
    drivers.AddDriver(&mouse);

    if (TEXT_MODE) {
        kbd.SetEventHandler(&console);
        mouse.SetEventHandler(&console);
        
    }
    else {
        kbd.SetEventHandler(&desktop);
        mouse.SetEventHandler(&desktop);
    }

    GraphicsContext* gfx;
    if (info->framebuffer_width > 400) {
        GraphicsContext g((uint32_t*)info->framebuffer_addr, info->framebuffer_width, info->framebuffer_height);
        gfx = &g;
    }
    else if (! TEXT_MODE) {
        VGAGraphicsMode g;
        g.Activate();
        gfx = &g;
    }

    drivers.ActivateAll();

    EnableInterrupts();

    while (true) {
        if (! TEXT_MODE) {
            desktop.Draw(gfx);
            gfx->PresentVSync();
        }
    }

    while (true);

}