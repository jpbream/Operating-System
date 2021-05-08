#ifndef _PARTITION_TABLE_H_
#define _PARTITION_TABLE_H_

#include "ata.h"
#include <stdint.h>

struct Partition
{
    uint8_t bootable;

    uint8_t startHead;
    uint8_t startSector : 6;
    uint16_t startCylinder : 10;

    uint8_t partitionId;

    uint8_t endHead;
    uint8_t endSector : 6;
    uint16_t endCylinder : 10;

    uint32_t startLba;
    uint32_t length;

} __attribute__((packed));

struct MasterBootRecord
{
    uint8_t bootloader[440];
    uint32_t signature;
    uint16_t unused;

    Partition partitions[4];

    uint16_t bootSignature;

} __attribute__((packed));

// this is the MS DOS style partition table
class PartitionTable
{
private:
    ATA* drive;
    MasterBootRecord mbr;

public:

    PartitionTable(ATA* drive);

    Partition GetPartition(int idx);

};

#endif