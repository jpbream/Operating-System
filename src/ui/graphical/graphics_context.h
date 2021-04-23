#ifndef _GRAPHICS_CONTEXT_H_
#define _GRAPHICS_CONTEXT_H_

#include <stdint.h>
#include "color.h"

//typedef VGAGraphicsMode GraphicsContext;

class GraphicsContext
{
protected:
    uint32_t* framebuffer;
    uint32_t* backbuffer;
    int width;
    int height;

public:
    GraphicsContext(uint32_t* framebuffer, int width, int height);

    inline virtual void PutPixel(int x, int y, Color color)
    {
        backbuffer[y * width + x] = color.value;
    }

    virtual void FillScreen(Color color);
    virtual void FillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color);
    virtual void PresentVSync();
};

#endif