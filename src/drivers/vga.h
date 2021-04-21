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

class VGAGraphicsMode : public VGA
{
private:

    uint8_t* memStart;
    static constexpr int WIDTH = 320;
    static constexpr int HEIGHT = 200;
    uint8_t frameBuffer[WIDTH * HEIGHT];

public:

    VGAGraphicsMode();

    void Activate() override;

    inline void PutPixel(uint32_t x, uint32_t y, uint8_t colorIndex)
    {
        memStart[y * WIDTH + x] = colorIndex;
    }

    void FillScreen(uint8_t color);
    void FillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t color);
    void PresentVSync();
};

class VGATextMode : public VGA
{
private:

    uint16_t* memStart;
    static constexpr int WIDTH = 80;
    static constexpr int HEIGHT = 25;

    static constexpr uint16_t BLANK = 0x0700;

public:

    VGATextMode();

    void Activate() override;

    inline void PutChar(uint32_t x, uint32_t y, char c)
    {
        *(memStart + (WIDTH * y) + x) = (*(memStart + (WIDTH * y) + x) & 0xFF00) | c;
    }

    inline void SetBackground(uint32_t x, uint32_t y, uint8_t color)
    {
        *(memStart + (WIDTH * y) + x) = (*(memStart + (WIDTH * y) + x) & 0x00FF) | color;
    }

    inline void Clear(uint32_t x, uint32_t y)
    {
        *(memStart + (WIDTH * y) + x) = BLANK;
    }

    void SwapColors(uint32_t x, uint32_t y);
};

#endif