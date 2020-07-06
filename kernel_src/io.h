#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

extern "C" void outb(uint16_t port, uint8_t byte);
extern "C" uint8_t inb(uint16_t port);

//extern "C" void outw(uint16_t port, uint16_t word);
//extern "C" uint16_t inw(uint16_t port);

//extern "C" void outd(uint16_t port, uint32_t dword);
//extern "C" uint32_t ind(uint16_t port);

extern "C" void io_wait();

#endif