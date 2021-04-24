#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>
#include <stdint.h>

// n must be a multiple of 16
extern "C" void *memcpy_sse(void *dest, const void * src, size_t n);

// n must be a multiple of 32
extern "C" void *memcpy(void *dest, const void * src, size_t n);


extern "C" void *memset(void *str, uint8_t b, size_t n);
extern "C" void *memsetw(void *str, uint16_t w, size_t n);
extern "C" void *memsetd(void *str, uint32_t d, size_t n);

#endif