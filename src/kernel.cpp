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
#include "sse.h"
#include "memory.h"

#define TEXT_MODE false

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
    SSE::Init();

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

    MemoryManager memoryManager(heap, memUpper);

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
        VGAGraphicsMode* g = new VGAGraphicsMode();
        g->Activate();
        gfx = g;
    }

    drivers.ActivateAll();

    CPUInfo cpu;
    printf("%s\n", cpu.VendorID());
    printf("SSE %s\n", cpu.QueryFeature(CPUInfo::FEAT_SSE) ? "Supported" : "Not Supported");
    printf("SSE2 %s\n", cpu.QueryFeature(CPUInfo::FEAT_SSE2) ? "Supported" : "Not Supported");
    printf("SSE3 %s\n", cpu.QueryFeature(CPUInfo::FEAT_SSE3) ? "Supported" : "Not Supported");
    printf("SSE4_1 %s\n", cpu.QueryFeature(CPUInfo::FEAT_SSE4_1) ? "Supported" : "Not Supported");
    printf("SSE4_2 %s\n", cpu.QueryFeature(CPUInfo::FEAT_SSE4_2) ? "Supported" : "Not Supported");
    printf("AVX %s\n", cpu.QueryFeature(CPUInfo::FEAT_AVX) ? "Supported" : "Not Supported");
    printf("AVX2 %s\n", cpu.QueryFeature(CPUInfo::FEAT_AVX2) ? "Supported" : "Not Supported");

    EnableInterrupts();

    while (true) {
        if (! TEXT_MODE) {
            desktop.Draw(gfx);
            gfx->PresentVSync();
        }
    }

    while (true);

}