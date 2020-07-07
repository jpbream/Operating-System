#include "port.h"

extern "C" void outb(uint16_t port, uint8_t byte);
extern "C" uint8_t inb(uint16_t port);

//extern "C" void outw(uint16_t port, uint16_t word);
//extern "C" uint16_t inw(uint16_t port);

//extern "C" void outd(uint16_t port, uint32_t dword);
//extern "C" uint32_t ind(uint16_t port);

extern "C" void io_wait();

void Write8(uint16_t port, uint8_t byte) {
    outb(port, byte);
}
void Write8Slow(uint16_t port, uint8_t byte) {
    outb(port, byte);
    io_wait();
}

Port::Port(uint16_t portId) : portId(portId) {}

void Port::Write8(uint8_t byte) {
    outb(portId, byte);
}

void Port::Write8Slow(uint8_t byte) {
    outb(portId, byte);
    io_wait();
}

uint8_t Port::Read8() {
    return inb(portId);
}