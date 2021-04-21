#ifndef _PCI_H_
#define _PCI_H_

#include <stdint.h>

#include "port.h"
#include "idt.h"
#include "driver.h"

enum BaseAddressRegisterType
{
    MemoryMap = 0,
    InputOutput = 1,
};

struct PCIBaseAddressRegister
{
    bool prefetchable;
    uint8_t* address;
    uint32_t size;
    BaseAddressRegisterType type;
};

struct PCIDevice 
{
    uint32_t portBase;
    uint32_t interrupt;

    uint8_t bus;
    uint8_t device;
    uint8_t function;

    uint16_t vendorId;
    uint16_t deviceId;
    uint8_t classId;
    uint8_t subclassId;
    uint8_t interfaceId;

    uint8_t revision;
};

class PCI 
{
private:
    Port dataPort;
    Port commandPort;

    PCIBaseAddressRegister GetBaseAddressRegister(uint8_t bus, uint8_t device, uint8_t function, uint16_t bar);

    // read data from a device. specify the device with the bus number, device number,
    // and function number. Specify the offset of the data to read with registerOffset
    uint32_t Read(uint8_t bus, uint8_t device, uint8_t function, uint8_t registerOffset);

    void Write(uint8_t bus, uint8_t device, uint8_t function, uint8_t registerOffset, uint32_t data);

    bool DeviceHasFunctions(uint8_t bus, uint8_t device);

    PCIDevice GetDevice(uint8_t bus, uint8_t device, uint8_t function);

    Driver* GetDriver(PCIDevice* device, IDT* idt);
    
public:
    PCI();
    ~PCI();

    void SelectDrivers(DriverManager* drivers, IDT* idt); 
};

#endif