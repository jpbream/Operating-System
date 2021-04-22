#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <stddef.h>
#include "memory_chunk.h"

class MemoryManager
{
protected:
    MemoryChunk* first;

public:
    static MemoryManager* activeMemoryManager;

    MemoryManager(size_t first, size_t size);

    void* malloc(size_t size);
    void free(void* ptr);
};

#endif