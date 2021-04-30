#ifndef _LINKED_MEMORY_HEAP_H_
#define _LINKED_MEMORY_HEAP_H_

#include "memory_heap.h"

class LinkedMemoryHeap : public MemoryHeap
{
private:

    struct MemoryBlock
    {
        uint32_t start;
        uint32_t span;
        MemoryBlock* next;
    } __attribute__((packed));

    MemoryBlock dummyHeadMem;
    MemoryBlock dummyTailMem;

    MemoryBlock* dummyHead;
    MemoryBlock* dummyTail;

    inline uint32_t GetNearestMultipleAtOrAfter(uint32_t atOrAfter, uint32_t multipleOf)
    {
        return (atOrAfter % multipleOf == 0) 
            ? (atOrAfter) 
            : (((atOrAfter / multipleOf) + 1) * multipleOf);
    }

public:
    LinkedMemoryHeap(uint32_t start, uint32_t span);

    void* AllocateAligned(size_t size, uint32_t alignment) override;

    void Free(void* ptr) override;

    uint32_t GetUsedSpace() override;
};

#endif