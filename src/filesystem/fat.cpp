#include "fat.h"
#include "print.h"

FAT::FAT(Partition* partition)
    : partition(partition)
{
    ReadBiosBlock();
}

void FAT::ReadBiosBlock()
{
    BiosParameterBlock32 biosBlock;
    partition->Read(partition->SectorStart(), (uint8_t*)&biosBlock, sizeof(BiosParameterBlock32));
    printf("Volume Label: %s\n", biosBlock.volumeLabel);

    uint32_t fatStart = partition->SectorStart() + biosBlock.numReservedSectors;
    uint32_t fatSize = biosBlock.tableSize;

    uint32_t dataStart = fatStart + fatSize * biosBlock.numFatCopies;
                                                                // cluster number has an offset of 2?
    uint32_t rootStart = dataStart + biosBlock.sectorsPerCluster * (biosBlock.rootCluster - 2);

    DirectoryEntryFat dirent[16];
    partition->Read(rootStart, (uint8_t*)&dirent[0], 16 * sizeof(DirectoryEntryFat));

    for (int i = 0; i < 16; ++i) {
        if (dirent[i].name[0] == '\0') {
            break;
        }

        if ((dirent[i].attributes & 0x0F) == 0x0F) {
            continue;
        }

        printf("Dir Name: ");
        for (int j = 0; j < 8; ++j) {
            putchar(dirent[i].name[j]);
        }
        putchar('\n');

        if ((dirent[i].attributes & 0x10) == 0x10) { // directory
            continue;
        }

        uint32_t fileCluster = ((uint32_t)dirent[i].firstClusterHi << 16)
                                | ((uint32_t)dirent[i].firstClusterLow);
        uint32_t fileSector = dataStart + (fileCluster - 2) * biosBlock.sectorsPerCluster;

        uint8_t buffer[512];
        partition->Read(fileSector, buffer, 512);
        buffer[dirent[i].size] = '\0';
        printf("    File Contents: %s\n", buffer);
    }
}