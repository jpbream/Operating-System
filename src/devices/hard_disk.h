#ifndef _HARD_DISK_H_
#define _HARD_DISK_H_

#include <stdint.h>

class HardDisk
{
public:
    virtual void Identify() = 0;
    virtual void Read(uint32_t sector, uint8_t* data, int count) = 0;
    virtual void Write(uint32_t sector, uint8_t* data, int count) = 0;
    virtual void Flush() = 0;
};

#endif