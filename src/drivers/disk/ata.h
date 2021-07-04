#ifndef _ATA_H_
#define _ATA_H_

#include "hard_disk.h"
#include "port.h"
#include <stdint.h>

#define SEC_SIZE 512

class ATA : public HardDisk
{
private:
    Port data16;
    Port error;
    Port sectorCount;
    Port lbaLow;
    Port lbaMid;
    Port lbaHigh;
    Port device;
    Port command;
    Port control;

    bool master;

public:
    ATA(uint16_t portBase, bool master);

    void Identify() override;
    void Read(uint32_t sector, uint8_t* data, int count) override;
    void Write(uint32_t sector, uint8_t* data, int count) override;
    void Flush() override;

};

#endif