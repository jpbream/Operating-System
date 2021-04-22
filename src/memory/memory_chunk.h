#ifndef _MEMORY_CHUNK_H_
#define _MEMORY_CHUNK_H_

#include <stddef.h>

struct MemoryChunk
{
    MemoryChunk* next;
    MemoryChunk* prev;
    bool allocated;
    size_t size;
};

#endif