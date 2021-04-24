#include "graphics_context.h"
#include "memory_manager.h"
#include "port.h"
#include "memory.h"

GraphicsContext::GraphicsContext(uint32_t* framebuffer, int width, int height)
: framebuffer(framebuffer), width(width), height(height)
{
    backbuffer = (uint32_t*)MemoryManager::activeMemoryManager->malloc(width * height * sizeof(uint32_t));
}

void GraphicsContext::FillScreen(Color color)
{
    uint64_t value = ((uint64_t)color.value << 32) | color.value;

    uint64_t* end = (uint64_t*)(backbuffer + width * height);
    uint64_t* traveller = (uint64_t*)backbuffer;

    memsetd(backbuffer, color.value, width * height * 4);
}

void GraphicsContext::FillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color color)
{
    uint32_t* scanlineStart = backbuffer + y * this->width + x;
    uint32_t* scanlineEnd = backbuffer + y * this->width +  + x + width;

    for (int row = 0; row < height; ++row) {

        uint32_t* traveller = scanlineStart;
        while (traveller != scanlineEnd) {
            *traveller = color.value;
            traveller++;
        }

        scanlineStart += this->width;
        scanlineEnd += this->width;
    }
}

void GraphicsContext::PresentVSync()
{
    // copy frame buffer to the VGA memory location
    uint64_t* end = (uint64_t*)(backbuffer + width * height);
    uint64_t* traveller = (uint64_t*)backbuffer;
    uint64_t* dest = (uint64_t*)framebuffer;

    // copying to the memory mapped framebuffer with avx doesn't work for some reason
    memcpy_sse(framebuffer, backbuffer, width * height * 4);
}