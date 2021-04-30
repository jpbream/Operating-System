#ifndef _PORT_H_
#define _PORT_H_

#include <stdint.h>

namespace Ports {
    void Write8(uint16_t port, uint8_t byte);
    void Write8Slow(uint16_t port, uint8_t byte);
}

class Port {

private:
    uint16_t portId;

public:
    Port(uint16_t portId);

    void Write8(uint8_t byte);
    void Write8Slow(uint8_t byte);

    void Write16(uint16_t word);

    void Write32(uint32_t dword);

    uint8_t Read8();
    uint16_t Read16();
    uint32_t Read32();

};

#endif