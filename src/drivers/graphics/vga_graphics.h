#ifndef _VGA_GRAPHICS_H_
#define _VGA_GRAPHICS_H_

#include <stdint.h>
#include "graphics_context.h"
#include "vga.h"

class VGAGraphicsMode : public VGA, public GraphicsContext
{
private:

    inline uint8_t LookupColor(Color color)
    {
        // byte order swapped so it goes ARGB
        switch (color.value) {
            case 0xFF000000:
                return 0;
            case 0xFF0000FF:
                return 1;
            case 0xFF00FF00:
                return 2;
            case 0xFFFFFFFF:
                return 0x3F;
            default:
                return 0;
        }
    }

public:

    VGAGraphicsMode();

    void Activate() override;

    inline void PutPixel(int x, int y, Color color) override
    {
        ((uint8_t*)backbuffer)[y * width + x] = LookupColor(color);
    }

    void FillScreen(Color color) override;
    void FillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color) override;
    void PresentVSync() override;
};

#endif