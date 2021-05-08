#ifndef _ATA_H_
#define _ATA_H_

#include "port.h"
#include <stdint.h>

#define SEC_SIZE 512

class ATA
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

    void Identify();
    void Read28(uint32_t sector, uint8_t* data, int count);
    void Write28(uint32_t sector, uint8_t* data, int count);
    void Flush();

};

#endif