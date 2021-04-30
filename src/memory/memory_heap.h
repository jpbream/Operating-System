#ifndef _MEMORY_HEAP_H_
#define _MEMORY_HEAP_H_

#include <stdint.h>
#include <stddef.h>

class MemoryHeap
{
protected:
    uint32_t lowAddress;
    uint32_t highAddress;
    uint32_t heapSize;

public:

    MemoryHeap(uint32_t start, uint32_t span);

    virtual void* Allocate(size_t size);
    virtual void* AllocateAligned(size_t size, uint32_t alignment) = 0;
    virtual void Free(void* ptr) = 0;

    virtual uint32_t GetUsedSpace() = 0;
};

#endif