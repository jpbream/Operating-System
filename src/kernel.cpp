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
#include "memory_manager.h"
#include "multiboot.h"
#include "graphics_context.h"

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

void* operator new(size_t size)
{
    return MemoryManager::activeMemoryManager->malloc(size);
}

void operator delete(void* ptr)
{
    MemoryManager::activeMemoryManager->free(ptr);
}

void operator delete(void* ptr, unsigned int)
{
    MemoryManager::activeMemoryManager->free(ptr);
}

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

    DriverManager drivers;

    PCI pci;
    pci.SelectDrivers(&drivers, &idt);

    // start the heap at 10 Mb
    size_t heap = 10 * 1024 * 1024;                     // padding
    uint32_t memUpper = info->mem_upper * 1024 - heap - 10 * 1024;

    printf("Heap Size: %d Mb\n", memUpper / 1024 / 1024);

    if (info->flags | MULTIBOOT_INFO_VBE_INFO) {
        printf("There is video info.\n");
    }
    if (info->flags | MULTIBOOT_INFO_FRAMEBUFFER_INFO) {
        printf("There is framebuffer info.\n");
    }

    printf("%x\n", info->framebuffer_addr);
    printf("%d %d\n", info->framebuffer_width, info->framebuffer_height);

    if (info->flags & (1<<6))
    {
      multiboot_memory_map_t *mmap;
      
      printf ("mmap_addr = %x, mmap_length = %d\n",
              (unsigned) info->mmap_addr, (unsigned) info->mmap_length);
      for (mmap = (multiboot_memory_map_t *) info->mmap_addr;
           (unsigned long) mmap < info->mmap_addr + info->mmap_length;
           mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                                    + mmap->size + sizeof (mmap->size)))
        printf (" size = %d, base_addr = %x,"
                " length = %d, type = %d\n",
                (unsigned) mmap->size,
                (unsigned) (mmap->addr),
                (unsigned) (mmap->len),
                (unsigned) mmap->type);
    }
    else {
        printf("Memory bit not set\n");
    }

    MemoryManager memoryManager(heap, memUpper);

    Desktop desktop(
        info->framebuffer_width > 400 ? info->framebuffer_width : 320, 
        info->framebuffer_width > 400 ? info->framebuffer_height : 200, 
        COLOR(0, 0, 255));

    Window window(&desktop, 50, 25, 50, 50, COLOR(0, 255, 0));
    Window window2(&desktop, 200, 25, 50, 50, COLOR(0, 255, 0));
    desktop.AddChild(&window);
    desktop.AddChild(&window2);
    //Console console;

    PS2Keyboard kbd(&idt);
    drivers.AddDriver(&kbd);
    kbd.SetEventHandler(&desktop);
    //kbd.SetEventHandler(&console);

    PS2Mouse mouse(&idt);
    drivers.AddDriver(&mouse);
    mouse.SetEventHandler(&desktop);
    //mouse.SetEventHandler(&console);

    GraphicsContext* gfx;
    if (info->framebuffer_width > 400) {
        //gfx = new GraphicsContext((uint32_t*)info->framebuffer_addr, info->framebuffer_width, info->framebuffer_height);
        GraphicsContext g((uint32_t*)info->framebuffer_addr, info->framebuffer_width, info->framebuffer_height);
        gfx = &g;
    }
    else {
        VGAGraphicsMode* g = new VGAGraphicsMode();
        g->Activate();
        gfx = g;
    }

    //VGATextMode vgaText;
    //vgaText.Activate();

    drivers.ActivateAll();

    CPUInfo* cpu = new CPUInfo();
    printf("%s\n", cpu->VendorID());
    delete cpu;

    EnableInterrupts();

    while (true) {
        desktop.Draw(gfx);
        gfx->PresentVSync();
    }

    while (true);

}