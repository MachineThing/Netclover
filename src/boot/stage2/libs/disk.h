#ifndef DISK_H
#define DISK_H

#include <stdint.h>

typedef struct {
    uint8_t id;
    uint16_t cylinders;
    uint16_t heads;
    uint16_t sectors;
} DISK;

uint16_t DISK_Init(DISK* disk, uint8_t driveNumber);
void DISK_LBA2CHS(DISK* disk, uint32_t lba, uint16_t* cylinderOut, uint16_t* sectorOut, uint16_t* headOut);
uint16_t DISK_ReadSectors(DISK* disk, uint32_t lba, uint8_t sectorsToRead, void* dataOut);

#endif