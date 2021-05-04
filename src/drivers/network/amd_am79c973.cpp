#include "amd_am79c973.h"
#include "print.h"

AMDAM79C973::AMDAM79C973(PCIDevice* device, IDT* idt)
: macAddress0(device->portBase), macAddress1(device->portBase + 0x02), macAddress2(device->portBase + 0x04),
  registerData(device->portBase + 0x10), registerAddress(device->portBase + 0x12), reset(device->portBase + 0x14), 
  busControlRegisterData(device->portBase + 0x16)
{
    idt->SetHandler(device->interrupt + HARDWARE_OFFSET, this);

    currentSendBuffer = 0;
    currentRecvBuffer = 0;

    uint64_t mac0 = macAddress0.Read16() % 256;
    uint64_t mac1 = macAddress0.Read16() / 256;
    uint64_t mac2 = macAddress1.Read16() % 256;
    uint64_t mac3 = macAddress1.Read16() / 256;
    uint64_t mac4 = macAddress2.Read16() % 256;
    uint64_t mac5 = macAddress2.Read16() / 256;

    uint64_t mac = mac5 << 40
                 | mac4 << 32
                 | mac3 << 24
                 | mac2 << 16
                 | mac1 << 8
                 | mac0;

    // set device to 32 bit mode
    registerAddress.Write16(20);
    busControlRegisterData.Write16(0x102);

    // stop reset
    registerAddress.Write16(0);
    registerData.Write16(0x04);

    // initialization block
    initBlock.mode = 0x0000; // promiscuous = false
    initBlock.reserved1 = 0;
    initBlock.numSendBuffers = 3; // 2^3 = 8 buffers
    initBlock.reserved2 = 0;
    initBlock.numRecvBuffers = 3;
    initBlock.physicalAddress = mac;
    initBlock.reserved3 = 0;
    initBlock.logicalAddress = 0;

    sendBufferDesc = (BufferDescriptor*)((((uint32_t)(&sendBufferDescMemory[0]) + 15) & ~((uint32_t)0xF)));
    initBlock.sendBufferDescAddress = (uint32_t)sendBufferDesc;

    receiveBufferDesc = (BufferDescriptor*)((((uint32_t)(&receiveBufferDescMemory[0]) + 15) & ~((uint32_t)0xF)));
    initBlock.recvBufferDescAddress = (uint32_t)receiveBufferDesc;

    for (int i = 0; i < 8; ++i) {
        sendBufferDesc[i].address = (((uint32_t)&sendBuffers[i]) + 15) & ~(uint32_t)0xF;
        sendBufferDesc[i].flags = 0xF7FF;
        sendBufferDesc[i].flags2 = 0;
        sendBufferDesc[i].available = 0;

        receiveBufferDesc[i].address = (((uint32_t)&receiveBuffers[i]) + 15) & ~(uint32_t)0xF;
        receiveBufferDesc[i].flags = 0xF7FF | 0x80000000;
        receiveBufferDesc[i].flags2 = 0;
        receiveBufferDesc[i].available = 0;
    }

    registerAddress.Write16(1);
    registerData.Write16((uint32_t)(&initBlock) & 0xFFFF);
    registerAddress.Write16(2);
    registerData.Write16(((uint32_t)(&initBlock) >> 16) & 0xFFFF);
}

void AMDAM79C973::Activate()
{
    registerAddress.Write16(0);
    registerData.Write16(0x41); // enable interrupts

    registerAddress.Write16(4);
    uint32_t temp = registerData.Read16();
    registerAddress.Write16(4);
    registerData.Write16(temp | 0xC00);

    registerAddress.Write16(0);
    registerData.Write16(0x42);
} 

int AMDAM79C973::Reset() 
{
    reset.Read16();
    reset.Write16(0);
    return 10; // wait for 10 ms
}

CPUState* AMDAM79C973::Handle(uint8_t interrupt, CPUState* regs) 
{
    printf("NETWORK INTERRUPT\n");

    registerAddress.Write16(0);
    uint32_t temp = registerData.Read16();

    if (temp & 0x8000) printf("General Error\n");
    if (temp & 0x2000) printf("Collision Error\n");
    if (temp & 0x1000) printf("Missed Frame\n");
    if (temp & 0x0800) printf("Memory Error\n");
    if (temp & 0x0400) {
        printf("Data Received\n");
        Receive();
    }
    if (temp & 0x0200) printf("Data Sent\n");

    registerAddress.Write16(0);
    registerData.Write16(temp); // acknowledge

    if (temp & 0x0100) printf("Init Done\n");

    return regs;
}

void AMDAM79C973::Send(uint8_t* buffer, int size)
{
    int sendDescriptor = currentSendBuffer;
    currentSendBuffer = (currentSendBuffer + 1) % 8; // cycle to the next send buffer

    if (size > 1518) { // limit the size we can send
        size = 1518;
    }

    // copy data from end to start
    for (uint8_t* src = buffer + size - 1, 
            *dst = (uint8_t*)(sendBufferDesc[sendDescriptor].address + size - 1);
            src >= buffer; src--, dst--)
    {
        *dst = *src;
    }

    sendBufferDesc[sendDescriptor].available = 0;
    sendBufferDesc[sendDescriptor].flags2 = 0;
    sendBufferDesc[sendDescriptor].flags = 0x8300F000 
                                            | ((uint16_t)((-size) & 0xFFF));
    registerAddress.Write16(0);
    registerData.Write16(0x48);
}

void AMDAM79C973::Receive()
{
    for(; (receiveBufferDesc[currentRecvBuffer].flags & 0x80000000) == 0 
            ; currentRecvBuffer = (currentRecvBuffer + 1) % 8) 
    {
        if (!(receiveBufferDesc[currentRecvBuffer].flags & 0x40000000)
            && (receiveBufferDesc[currentRecvBuffer].flags & 0x03000000) == 0x03000000)
        {
            uint32_t size = receiveBufferDesc[currentRecvBuffer].flags & 0xFFF;
            if (size > 64) {
                size -= 4; // remove the checksum
            }

            uint8_t* buffer = (uint8_t*)(receiveBufferDesc[currentRecvBuffer].address);
            printf("DATA: %s\n", buffer);
        }

        receiveBufferDesc[currentRecvBuffer].flags2 = 0;
        receiveBufferDesc[currentRecvBuffer].flags = 0x8000F7FF;
    }
}