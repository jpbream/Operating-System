#include "partition_table.h"

PartitionTable::PartitionTable(ATA* drive)
{
    this->drive = drive;
    drive->Read28(0, (uint8_t*)&mbr, sizeof(MasterBootRecord));
}

Partition PartitionTable::GetPartition(int idx)
{
    return mbr.partitions[idx];
}