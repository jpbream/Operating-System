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
