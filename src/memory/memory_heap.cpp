#include "memory_heap.h"

MemoryHeap::MemoryHeap(uint32_t start, uint32_t span)
{
    lowAddress = start;
    highAddress = start + span;
    heapSize = span;
}

void* MemoryHeap::Allocate(size_t size)
{
    return AllocateAligned(size, 1);
}