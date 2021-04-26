#include "page_manager.h"
#include "print.h"

extern "C" void EnablePaging(PageDirectoryTable* table);

PageManager::PageManager()
{

}

void PageManager::InitKernelPageDirectory(PageDirectory* dir, uint32_t tableAddress)
{
    dir->present = 1;
    dir->rw = 1;
    dir->privilege = 0;
    dir->writeThrough = 1;
    dir->cacheDisable = 0;
    dir->accessed = 0;
    dir->pageSize = 0;
    dir->pageTableAddress = tableAddress;
}

void PageManager::InitKernelPage(Page* page, uint32_t pageAddress)
{
    page->present = 1;
    page->rw = 1;
    page->privilege = 0;
    page->writeThrough = 1;
    page->cacheDisable = 0;
    page->accessed = 0;
    page->dirty = 0;
    page->global = 0;
    page->frameAddress = pageAddress;
}

void PageManager::Activate()
{
    for (int i = 0; i < 1024; ++i) { // zero out the kernel page directory
        *(uint32_t*)&(kernelDirectory.directories[i]) = 0;
    }

    for (int j = 0; j < 1024; ++j) {
    for (int i = 0; i < 1024; ++i) { // zero out the first kernel page table
        *(uint32_t*)&(tables[j].pages[i]) = 0;
    }
    }

    for (int i = 0; i < 1024; ++i) {
        InitKernelPageDirectory(&kernelDirectory.directories[i], (uint32_t)(&tables[i]) >> 12);
    }

    for (int j = 0; j < 1024; ++j) {
    for (int i = 0; i < 1024; ++i) {
        InitKernelPage(&(tables[j].pages[i]), 0x1000 * j + i);
    }
    }

    printf("Starting paging\n");
    EnablePaging(&kernelDirectory);
}