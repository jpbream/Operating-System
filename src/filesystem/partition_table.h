#ifndef _PARTITION_TABLE_H_
#define _PARTITION_TABLE_H_

#include "hard_disk.h"
#include <stdint.h>

class PartitionTable;
class Partition;

// this is the MS DOS style partition table
class PartitionTable
{
    friend class Partition;
private:

    struct PartitionEntry
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

        PartitionEntry partitions[4];

        uint16_t bootSignature;

    } __attribute__((packed));

    HardDisk* drive;
    MasterBootRecord mbr;

public:

    PartitionTable(HardDisk* drive);
    ~PartitionTable();
};

class Partition
{
    friend class PartitionTable;
private:
    PartitionTable::PartitionEntry partition;
    HardDisk* disk; 

public:

    Partition(PartitionTable* table, int idx);
    ~Partition();

    bool Write(uint32_t sector, uint8_t* data, int count);
    bool Read(uint32_t sector, uint8_t* data, int count);

    uint32_t SectorStart();
    uint32_t SectorSpan();

};

#endif