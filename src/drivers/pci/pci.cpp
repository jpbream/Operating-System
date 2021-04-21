#include "pci.h"
#include "print.h"

PCI::PCI()
    : dataPort(0xCFC), commandPort(0xCF8)
{ }

PCI::~PCI()
{ }

uint32_t PCI::Read(uint8_t bus, uint8_t device, uint8_t function, uint8_t registerOffset)
{
    uint32_t id = 
        0x01 << 31 |
        (bus & 0xFF) << 16 |
        (device & 0x1F) << 11 |
        (function & 0x07) << 8 |

        // this masks off the last two bits of register offset,
        // which limits us to read at 4 byte intervals
        (registerOffset & 0xFC);

    commandPort.Write32(id);
    uint32_t result = dataPort.Read32();

    // return only the bytes starting at registerOffset
    return result >> (8 * (registerOffset % 4));
}

void PCI::Write(uint8_t bus, uint8_t device, uint8_t function, uint8_t registerOffset, uint32_t data)
{
    uint32_t id = 
        0x01 << 31 |
        (bus & 0xFF) << 16 |
        (device & 0x1F) << 11 |
        (function & 0x07) << 8 |
        (registerOffset & 0xFC);

    commandPort.Write32(id);
    dataPort.Write32(data);
}

bool PCI::DeviceHasFunctions(uint8_t bus, uint8_t device)
{
    // Read 7th bit of address 0x0E
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void PCI::SelectDrivers(DriverManager* drivers, IDT* idt)
{
    for (int bus = 0; bus < 8; ++bus) {
        for (int device = 0; device < 32; ++device) {
            
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for (int function = 0; function < numFunctions; ++function) {
                
                PCIDevice pciDevice = GetDevice(bus, device, function);

                // if device does not exist, vendor Id will be 0 or all 1's
                if (pciDevice.vendorId == 0 || pciDevice.vendorId == 0xFFFF) {
                    continue;
                }

                for (int barNum = 0; barNum < 6; ++barNum) {
                    PCIBaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    
                    if (bar.address && bar.type == InputOutput) {
                        pciDevice.portBase = (uint32_t)bar.address;
                    }

                    Driver* driver = GetDriver(&pciDevice, idt);
                    if (driver) {
                        drivers->AddDriver(driver);
                    }
                }
            }
        }
    }
}

PCIDevice PCI::GetDevice(uint8_t bus, uint8_t device, uint8_t function)
{
    PCIDevice pciDevice;

    pciDevice.bus = bus;
    pciDevice.device = device;
    pciDevice.function = function;

    pciDevice.vendorId = Read(bus, device, function, 0x00);
    pciDevice.deviceId = Read(bus, device, function, 0x02);
    pciDevice.classId = Read(bus, device, function, 0x0B);
    pciDevice.subclassId = Read(bus, device, function, 0x0A);
    pciDevice.interfaceId = Read(bus, device, function, 0x09);

    pciDevice.revision = Read(bus, device, function, 0x08);

    pciDevice.interrupt = Read(bus, device, function, 0x3C);

    return pciDevice;
}

PCIBaseAddressRegister PCI::GetBaseAddressRegister(uint8_t bus, uint8_t device, uint8_t function, uint16_t barNum)
{
    PCIBaseAddressRegister bar;

    // gives information about how many BAR's there are
    uint32_t headerType = Read(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - 4 * headerType;

    if (barNum >= maxBARs) {
        bar.address = 0;
        return bar;
    }

    uint32_t barValue = Read(bus, device, function, 0x10 + 4 * barNum);

    // I/O or Memory Mapped is indicated by the first bit
    bar.type = (barValue & 0x01) ? InputOutput : MemoryMap;

    if (bar.type == MemoryMap) {
        switch ((barValue >> 1) & 0x03) {
            case 0x00:
                break;
            case 0x01:
                break;
            case 0x02:
                break;
        }

        bar.prefetchable = (barValue >> 3) & 0x01 == 0x01;
    }
    else {
        // everything but the last two bits make up the address
        bar.address = (uint8_t*)(barValue & ~0x03);
        bar.prefetchable = false;
    }

    return bar;
}

Driver* PCI::GetDriver(PCIDevice* device, IDT* idt)
{
    switch (device->classId) {
        case 0x03: // graphics device
            switch (device->subclassId) {
                case 0x00: // VGA
                    break;
            }
        break;
    }

    return 0;
}
