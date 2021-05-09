#include "fat.h"
#include "print.h"

FAT::FAT(ATA* drive, Partition partition)
{
    this->drive = drive;
    this->partition = partition;

    ReadBiosBlock();
}

void FAT::ReadBiosBlock()
{
    BiosParameterBlock32 biosBlock;
    drive->Read28(partition.startLba, (uint8_t*)&biosBlock, sizeof(BiosParameterBlock32));
    printf("Volume Label: %s\n", biosBlock.volumeLabel);

    uint32_t fatStart = partition.startLba + biosBlock.numReservedSectors;
    uint32_t fatSize = biosBlock.tableSize;

    uint32_t dataStart = fatStart + fatSize * biosBlock.numFatCopies;
                                                                // cluster number has an offset of 2?
    uint32_t rootStart = dataStart + biosBlock.sectorsPerCluster * (biosBlock.rootCluster - 2);

    DirectoryEntryFat dirent[16];
    drive->Read28(rootStart, (uint8_t*)&dirent[0], 16 * sizeof(DirectoryEntryFat));

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

        uint8_t buffer[SEC_SIZE];
        drive->Read28(fileSector, buffer, 512);
        buffer[dirent[i].size] = '\0';
        printf("    File Contents: %s\n", buffer);
    }
}