#include "partition_table.h"

PartitionTable::PartitionTable(HardDisk* drive)
{
    this->drive = drive;
    drive->Read(0, (uint8_t*)&mbr, sizeof(MasterBootRecord));
}

PartitionTable::~PartitionTable()
{
    drive->Flush();
}

Partition::Partition(PartitionTable* table, int idx)
{
    this->disk = table->drive;
    this->partition = table->mbr.partitions[idx];
}

Partition::~Partition()
{
    disk->Flush();
}

bool Partition::Write(uint32_t sector, uint8_t* data, int count)
{
    if (sector == 0 || sector < partition.startLba || sector > partition.startLba + partition.length) {
        return false;
    }

    disk->Write(sector, data, count);
    return true;
}

bool Partition::Read(uint32_t sector, uint8_t* data, int count)
{
    if (sector < partition.startLba || sector > partition.startLba + partition.length) {
        return false;
    }

    disk->Read(sector, data, count);
    return true;
}

uint32_t Partition::SectorStart()
{
    return partition.startLba;
}

uint32_t Partition::SectorSpan()
{
    return partition.length;
}