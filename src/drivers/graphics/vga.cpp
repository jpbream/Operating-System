#include "vga.h"
#include "print.h"

VGA::VGA() :
    miscPort(0x3C2),
    crtcIndexPort(0x3D4),
    crtcDataPort(0x3D5),
    sequencerIndexPort(0x3C4),
    sequencerDataPort(0x3C5),
    graphicsControllerIndexPort(0x3CE),
    graphicsControllerDataPort(0x3CF),
    attributeControllerIndexPort(0x3C0),
    attributeControllerReadPort(0x3C1),
    attributeControllerWritePort(0x3C0),
    attributeControllerResetPort(0x3DA)
{ }

void VGA::WriteRegisters(uint8_t* registers)
{
    miscPort.Write8(*(registers++));

    for (uint8_t i = 0; i < 5; ++i) {
        sequencerIndexPort.Write8(i);
        sequencerDataPort.Write8(*(registers++));
    }

    // unlock the crt
    crtcIndexPort.Write8(0x03);
    crtcDataPort.Write8(crtcDataPort.Read8() | 0x80);
    crtcIndexPort.Write8(0x11);
    crtcDataPort.Write8(crtcDataPort.Read8() & ~0x80);

    registers[0x03] |= 0x80;
    registers[0x11] &= ~0x80;

    for (uint8_t i = 0; i < 25; ++i) {
        crtcIndexPort.Write8(i);
        crtcDataPort.Write8(*(registers++));
    }

    for (uint8_t i = 0; i < 9; ++i) {
        graphicsControllerIndexPort.Write8(i);
        graphicsControllerDataPort.Write8(*(registers++));
    }

    for (uint8_t i = 0; i < 21; ++i) {

        // index and write use the same port. VGA swaps their function
        // every time a value is written
        // reading from the Reset port forces it back to index state

        attributeControllerResetPort.Read8();
        attributeControllerIndexPort.Write8(i);
        attributeControllerWritePort.Write8(*(registers++));
    }

    attributeControllerResetPort.Read8();
    attributeControllerIndexPort.Write8(0x20);
}

uint8_t* VGA::GetFrameBufferSegment()
{
    graphicsControllerIndexPort.Write8(0x06);
    uint8_t segmentNumber = (graphicsControllerDataPort.Read8() >> 2) & 0x03;

    switch (segmentNumber) {
        default:
        case 0x00: return (uint8_t*)0x00000;
        case 0x01: return (uint8_t*)0xA0000;
        case 0x02: return (uint8_t*)0xB0000;
        case 0x03: return (uint8_t*)0xB8000;
    }
}

VGAGraphicsMode::VGAGraphicsMode() : VGA() {}

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
    memStart = GetFrameBufferSegment();
}

void VGAGraphicsMode::FillScreen(uint8_t color)
{
    uint64_t value = ((uint64_t)color << 56) | ((uint64_t)color << 48) | ((uint64_t)color << 40) |
                     ((uint64_t)color << 32) | ((uint64_t)color << 24) | ((uint64_t)color << 16) |
                     ((uint64_t)color << 8)  | (uint64_t)color;

    uint64_t* end = (uint64_t*)(frameBuffer + WIDTH * HEIGHT);
    uint64_t* traveller = (uint64_t*)frameBuffer;

    while (traveller != end) {
        *traveller = value;
        traveller++;
    }
}

void VGAGraphicsMode::FillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t color)
{
    uint8_t* scanlineStart = frameBuffer + y * WIDTH + x;
    uint8_t* scanlineEnd = frameBuffer + y * WIDTH +  + x + width;

    for (int row = 0; row < height; ++row) {

        uint8_t* traveller = scanlineStart;
        while (traveller != scanlineEnd) {
            *traveller = color;
            traveller++;
        }

        scanlineStart += WIDTH;
        scanlineEnd += WIDTH;
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
    uint64_t* end = (uint64_t*)(frameBuffer + WIDTH * HEIGHT);
    uint64_t* traveller = (uint64_t*)frameBuffer;
    uint64_t* dest = (uint64_t*)memStart;

    while (traveller != end) {
        *dest++ = *traveller++;
    }
}

VGATextMode::VGATextMode() : VGA() {}

void VGATextMode::Activate()
{
    unsigned char g_80x25_text[] =
    {
        /* MISC */
        0x67,
        /* SEQ */
        0x03, 0x00, 0x03, 0x00, 0x02,
        /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
        0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
        0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
        0xFF,
        /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
        0xFF,
        /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x0C, 0x00, 0x0F, 0x08, 0x00
    };

    WriteRegisters(g_80x25_text);

    // disable the cursor
    crtcIndexPort.Write8(0x0A);
    crtcDataPort.Write8(0x20);

    memStart = (uint16_t*)GetFrameBufferSegment();
}

void VGATextMode::SwapColors(uint32_t x, uint32_t y)
{
    uint16_t lower = *(memStart + y * WIDTH + x) & 0x0F00;
	uint16_t upper = *(memStart + y * WIDTH + x) & 0xF000;

	uint16_t newUpper = lower << 4;
	uint16_t newLower = upper >> 4;

	*(memStart + y * WIDTH + x) &= 0x00FF;
	*(memStart + y * WIDTH + x) |= (newUpper | newLower);
}