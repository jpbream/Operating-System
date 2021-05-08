#include "ata.h"
#include "print.h"

ATA::ATA(uint16_t portBase, bool master)
    : data16(portBase),
      error(portBase + 1),
      sectorCount(portBase + 2),
      lbaLow(portBase + 3),
      lbaMid(portBase + 4),
      lbaHigh(portBase + 5),
      device(portBase + 6),
      command(portBase + 7),
      control(portBase + 0x206)
{
    this->master = master;
}

void ATA::Identify()
{
    device.Write8(master ? 0xA0 : 0xB0);
    control.Write8(0);

    device.Write8(0xA0);
    uint8_t status = command.Read8();
    if (status == 0xFF) {
        printf("Not present.\n");
        return; // no device on this bus
    }

    device.Write8(master ? 0xA0 : 0xB0);
    sectorCount.Write8(0);
    lbaLow.Write8(0);
    lbaMid.Write8(0);
    lbaHigh.Write8(0);
    command.Write8(0xEC); // identify command

    status = command.Read8();
    if (status == 0x00) {
        printf("Not present.\n");
        return; // no device
    }

    while (status & 0x80 && !(status & 0x01)) {
        status = command.Read8();
    }

    if (status & 0x01) {
        printf("ERROR\n");
        return;
    }

    for (uint16_t i = 0; i < 256; ++i) {
        uint16_t data = data16.Read16();

        char* str = "  \0";
        str[0] = (data >> 8) & 0x00FF;
        str[1] = data & 0x00FF;
        printf("%s", str);
    }
    printf("\n");
}

void ATA::Read28(uint32_t sector, uint8_t* data, int count)
{
    if (sector & 0xF0000000 || count > SEC_SIZE) {
        return;
    }

    device.Write8((master ? 0xE0 : 0xF0) | (sector & 0x0F000000) >> 24); // top 4 bits of address go in device port
    error.Write8(0);
    sectorCount.Write8(1);
    lbaLow.Write8(sector & 0xFF);
    lbaMid.Write8((sector & 0xFF00) >> 8);
    lbaHigh.Write8((sector & 0xFF0000) >> 16);
    command.Write8(0x20); // read command

    uint8_t status = command.Read8();
    while (status & 0x80 && !(status & 0x01)) {
        status = command.Read8();
    }

    printf("Reading: ");

    for (int i = 0; i < count; i += 2) {
        uint16_t input = data16.Read16();

        char* str = "  \0";
        str[1] = (input >> 8) & 0x00FF;
        str[0] = input & 0x00FF;
        printf("%s", str);

        data[i] = input & 0xFF;
        if (i + 1 < count) {
            data[i + 1] = (input >> 8) & 0x00FF;
        }
    }

    // disk always expects a full sector
    for (int i = count + (count % 2); i < SEC_SIZE; ++i) {
        data16.Read16();
    }

    printf("\n");
}

void ATA::Write28(uint32_t sector, uint8_t* data, int count)
{
    if (sector & 0xF0000000 || count > SEC_SIZE) {
        return;
    }

    device.Write8((master ? 0xE0 : 0xF0) | (sector & 0x0F000000) >> 24); // top 4 bits of address go in device port
    error.Write8(0);
    sectorCount.Write8(1);
    lbaLow.Write8(sector & 0xFF);
    lbaMid.Write8((sector & 0xFF00) >> 8);
    lbaHigh.Write8((sector & 0xFF0000) >> 16);
    command.Write8(0x30); // write command

    printf("Writing: ");

    for (int i = 0; i < count; i += 2) {
        uint16_t output = data[i];

        if (i + 1 < count) {
            output |= data[i + 1] << 8;
        }

        char* str = "  \0";
        str[1] = (output >> 8) & 0x00FF;
        str[0] = output & 0x00FF;
        printf("%s", str);

        data16.Write16(output);
    }

    // disk always expects a full sector
    for (int i = count + (count % 2); i < SEC_SIZE; ++i) {
        data16.Write16(0x0000);
    }

    printf("\n");
}

void ATA::Flush()
{
    device.Write8(master ? 0xE0 : 0xF0);
    command.Write8(0xE7); // flush

    printf("Flushing: ");

    uint8_t status = command.Read8();
    while (status & 0x80 && !(status & 0x01)) {
        status = command.Read8();
    }

    if (status & 0x01) {
        printf("ERROR\n");
        return;
    }

    printf("\n");
}