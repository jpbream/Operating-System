#ifndef _VGA_H_
#define _VGA_H_

#include <stdint.h>
#include "port.h"

class VGA
{
protected:
    Port miscPort;
    Port crtcIndexPort;
    Port crtcDataPort;
    Port sequencerIndexPort;
    Port sequencerDataPort;
    Port graphicsControllerIndexPort;
    Port graphicsControllerDataPort;
    Port attributeControllerIndexPort;
    Port attributeControllerReadPort;
    Port attributeControllerWritePort;
    Port attributeControllerResetPort;

    void WriteRegisters(uint8_t* registers);
    uint8_t* GetFrameBufferSegment();

public:
    VGA();
    virtual void Activate() = 0;
};

#endif