#ifndef _VGA_TEXT_H_
#define _VGA_TEXT_H_

#include "vga.h"
#include <stdint.h>

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