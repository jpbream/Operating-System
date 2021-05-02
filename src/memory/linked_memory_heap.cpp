#include "linked_memory_heap.h"

LinkedMemoryHeap::LinkedMemoryHeap(uint32_t start, uint32_t span)
: MemoryHeap(start, span)
{
    dummyTailMem = (MemoryBlock){ highAddress + 1, 0, 0 };
    dummyTail = &dummyTailMem;

    dummyHeadMem = (MemoryBlock){ lowAddress - 1, 0, dummyTail };
    dummyHead = &dummyHeadMem;
}

void* LinkedMemoryHeap::AllocateAligned(size_t size, uint32_t alignment)
{   
    // size of the block requested + metadata storage
    uint32_t effectiveSize = size + sizeof(MemoryBlock);

    // we have to traverse the list and look for space
    MemoryBlock* traveller = dummyHead;

    // where we will create the new block
    MemoryBlock* newBlockLocation = 0;

    // loop through the chain
    while (traveller != dummyTail) {

        // if we were to insert the block here, calculate the memory address it would start at
        MemoryBlock* possibleBlockLocation = (MemoryBlock*)(traveller->start + traveller->span + 1);
        possibleBlockLocation = (MemoryBlock*)GetNearestMultipleAtOrAfter((uint32_t)possibleBlockLocation, alignment);

        // calculate how much space is between newBlockLocation and the next node
        uint32_t spaceAvailable = (uint32_t)traveller->next->start - (uint32_t)possibleBlockLocation;

        // if enough space is available here to create the block, exit the loop
        if (spaceAvailable >= effectiveSize) {
            newBlockLocation = possibleBlockLocation;
            break;
        }

        traveller = traveller->next;
    }

    // if we could not find space for the new block, return null
    if (newBlockLocation == 0) {
        return 0;
    }

    // set up the new block
    *newBlockLocation = (MemoryBlock){ (uint32_t)newBlockLocation, effectiveSize, traveller->next };

    // link the new block into the chain
    traveller->next = newBlockLocation;

    // return pointer to byte after the block metadata
    return (void*)newBlockLocation + sizeof(MemoryBlock);
    
}

void LinkedMemoryHeap::Free(void* ptr)
{
    // ptr should be a pointer to the data section of a block, find the
    // actual start address of that block
    MemoryBlock* blockToFree = (MemoryBlock*)(ptr - sizeof(MemoryBlock));

    // head is a dummy node that cannot be freed
    if (ptr == dummyHead) {
        return;
    }

    MemoryBlock* traveller = dummyHead;
    while (traveller != dummyTail) {

        // if the next block is the block to free
        if (traveller->next == blockToFree) {

            // unlink it from the chain
            traveller->next = traveller->next->next;
            return;
        }
        traveller = traveller->next;
    } 
}

uint32_t LinkedMemoryHeap::GetUsedSpace()
{
    if (dummyHead == 0) {
        return 0;
    }

    uint32_t used = 0;

    MemoryBlock* traveller = dummyHead;
    while (traveller->next != 0) {
        used += traveller->span - sizeof(MemoryBlock);
    }

    return used;
}