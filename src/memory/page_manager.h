#ifndef _PAGE_MANAGER_H_
#define _PAGE_MANAGER_H_

#include <stdint.h>

struct BootloaderMMap {
    uint32_t base;
    uint32_t limit;
    uint8_t type;
};

struct PageDirectory {

    uint32_t present : 1;
    uint32_t rw : 1; // 1 means read/write
    uint32_t privilege : 1; // 0 is kernel only
    uint32_t writeThrough : 1;
    uint32_t cacheDisable : 1;
    uint32_t accessed : 1;
    uint32_t ignoredBit : 1;
    uint32_t pageSize : 1; // 0 is 4kb
    uint32_t unusedBits : 4;
    uint32_t pageTableAddress : 20;

} __attribute__((packed));

struct PageDirectoryTable {
    PageDirectory directories[1024];
} __attribute__((aligned(4096)));

struct Page {

    uint32_t present : 1;
    uint32_t rw : 1; // 1 means read/write
    uint32_t privilege : 1; // 0 is kernel only
    uint32_t writeThrough : 1;
    uint32_t cacheDisable : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t unusedBits1 : 1;
    uint32_t global : 1;
    uint32_t unusedBits2 : 3;
    uint32_t frameAddress : 20;

} __attribute__((packed));

struct PageTable {
    Page pages[1024];
} __attribute__((aligned(4096)));

class PageManager {

private:
    enum BootloaderMMapType {
        AVAILABLE = 1,
        RESERVED = 2,
        RECLAIMABLE = 3,
        NVS = 4, // reserved
        BADRAM = 5
    };

    PageDirectoryTable kernelDirectory;
    PageTable tables[1024];
    PageDirectoryTable* currentDirectory;

    void InitKernelPageDirectory(PageDirectory* dir, uint32_t tableAddress);
    void InitKernelPage(Page* page, uint32_t pageAddress);

public:

    PageManager();
    void Activate();

};

#endif