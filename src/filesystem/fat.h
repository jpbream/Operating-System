#ifndef _FAT_H_
#define _FAT_H_

#include "partition_table.h"

class FAT
{
private:

    struct BiosParameterBlock32
    {
        uint8_t jumpInstruction[3];
        uint8_t softwareFormatterName[8];
        uint16_t bytesPerSector;
        uint8_t sectorsPerCluster;
        uint16_t numReservedSectors;
        uint8_t numFatCopies;
        uint16_t numRootDirEntries;
        uint16_t totalSectors;
        uint8_t mediaType;
        uint16_t fat16SectorCount;
        uint16_t sectorsPerTrack;
        uint16_t headCount;
        uint32_t hiddenSectors;
        uint32_t totalSectorCount;

        uint32_t tableSize;
        uint16_t extFlags;
        uint16_t fatVersion;
        uint32_t rootCluster;
        uint16_t fatInfo;
        uint16_t backupSector;
        uint8_t reserved0[12];
        uint8_t driveNumber;
        uint8_t reserved;
        uint8_t bootSignature;
        uint32_t volumeId;
        uint8_t volumeLabel[11];
        uint8_t fatTypeLabel[8];

    } __attribute__((packed));

    struct DirectoryEntryFat
    {
        uint8_t name[8];
        uint8_t extension[3];
        uint8_t attributes;
        uint8_t reserved;
        uint8_t cTimeTenth;
        uint16_t creationTime;
        uint16_t creationDate;
        uint16_t accessTime;
        uint16_t firstClusterHi;
        uint16_t writeTime;
        uint16_t writeDate;
        uint16_t firstClusterLow;
        uint32_t size;

    } __attribute__((packed));

    Partition* partition;

    void ReadBiosBlock();

public:
    FAT(Partition* partition);
};

#endif