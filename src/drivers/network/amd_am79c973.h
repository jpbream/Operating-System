#ifndef _AMD_AM79C973_H_
#define _AMD_AM79C973_H_

#include "pci.h"
#include "idt.h"
#include "driver.h"
#include "interrupt_handler.h"
#include "port.h"

#include <stdint.h>

class AMDAM79C973 : public Driver, public InterruptHandler
{
private:

    struct InitializationBlock
    {
        uint16_t mode;
        unsigned reserved1 : 4;
        unsigned numSendBuffers : 4;
        unsigned reserved2 : 4;
        unsigned numRecvBuffers : 4;
        uint64_t physicalAddress : 48;
        uint16_t reserved3;
        uint64_t logicalAddress;
        uint32_t recvBufferDescAddress;
        uint32_t sendBufferDescAddress;
    } __attribute__((packed));

    struct BufferDescriptor
    {
        uint32_t address;
        uint32_t flags;
        uint32_t flags2;
        uint32_t available;
    } __attribute__((packed));

    Port macAddress0;
    Port macAddress1;
    Port macAddress2;

    Port registerData;
    Port registerAddress;
    Port reset;
    Port busControlRegisterData;

    InitializationBlock initBlock;

    BufferDescriptor* sendBufferDesc;
    uint8_t sendBufferDescMemory[2048 + 15];
    uint8_t sendBuffers[2 * 1024 + 15][8];
    uint8_t currentSendBuffer;

    BufferDescriptor* receiveBufferDesc;
    uint8_t receiveBufferDescMemory[2048 + 15];
    uint8_t receiveBuffers[2 * 1024 + 15][8];
    uint8_t currentRecvBuffer;

public:
    AMDAM79C973(PCIDevice* device, IDT* idt);

    void Activate() override;
    int Reset() override;
    CPUState* Handle(uint8_t interrupt, CPUState* regs) override;

};

#endif