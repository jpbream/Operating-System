#include "vga_graphics.h"
#include "memory_heap.h"

extern MemoryHeap* kernelHeap;

VGAGraphicsMode::VGAGraphicsMode() 
: VGA(), GraphicsContext(0, 320, 200)
{
    kernelHeap->Free(backbuffer);
    backbuffer = (uint32_t*)kernelHeap->Allocate(width * height * sizeof(uint8_t));
}

void VGAGraphicsMode::Activate()
{
    // find mode setting sequences at
    // https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c

    uint8_t g_320x200x256[] =
    {
        /* MISC */
        0x63,
        /* SEQ */
        0x03, 0x01, 0x0F, 0x00, 0x0E,
        /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
        0xFF,
        /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,
        /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00,	0x00
    };

    WriteRegisters(g_320x200x256);
    framebuffer = (uint32_t*)GetFrameBufferSegment();
}

void VGAGraphicsMode::FillScreen(Color rgbColor)
{
    uint8_t color = LookupColor(rgbColor);
    uint64_t value = ((uint64_t)color << 56) | ((uint64_t)color << 48) | ((uint64_t)color << 40) |
                     ((uint64_t)color << 32) | ((uint64_t)color << 24) | ((uint64_t)color << 16) |
                     ((uint64_t)color << 8)  | (uint64_t)color;

    uint64_t* end = (uint64_t*)((uint8_t*)backbuffer + width * height);
    uint64_t* traveller = (uint64_t*)backbuffer;

    while (traveller != end) {
        *traveller = value;
        traveller++;
    }
}

void VGAGraphicsMode::FillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Color rgbColor)
{
    uint8_t color = LookupColor(rgbColor);
    uint8_t* scanlineStart = (uint8_t*)backbuffer + y * this->width + x;
    uint8_t* scanlineEnd = (uint8_t*)backbuffer + y * this->width +  + x + width;

    for (int row = 0; row < height; ++row) {

        uint8_t* traveller = scanlineStart;
        while (traveller != scanlineEnd) {
            *traveller = color;
            traveller++;
        }

        scanlineStart += this->width;
        scanlineEnd += this->width;
    }
}

void VGAGraphicsMode::PresentVSync()
{
    int result;
    do {
        result = attributeControllerResetPort.Read8();
    } while (result != 9);
    // wait until the bits of the vertical retrace register are on
    // https://web.stanford.edu/class/cs140/projects/pintos/specs/freevga/vga/extreg.htm

    // copy frame buffer to the VGA memory location
    uint64_t* end = (uint64_t*)((uint8_t*)backbuffer + width * height);
    uint64_t* traveller = (uint64_t*)backbuffer;
    uint64_t* dest = (uint64_t*)framebuffer;

    while (traveller != end) {
        *dest++ = *traveller++;
    }
}