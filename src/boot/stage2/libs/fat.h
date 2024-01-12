#ifndef FAT_H
#define FAT_H

#include <stdint.h>

typedef struct 
{
    uint8_t _BootJumpInstruction[3];
    uint8_t OemIdentifier[8];
    uint16_t BytesPerSector;
    uint8_t SectorsPerCluster;
    uint16_t ReservedSectors;           // Bootloader sectors, including this one (Note: STAGE2_SIZE comes from stage2/size.inc, an automatically generated file)
    uint8_t FatCount;
    uint16_t DirEntryCount;
    uint16_t TotalSectors;
    uint8_t MediaDescriptorType;
    uint16_t SectorsPerFat;
    uint16_t SectorsPerTrack;
    uint16_t Heads;
    uint32_t HiddenSectors;
    uint32_t LargeSectorCount;

    // extended boot record
    uint8_t DriveNumber;
    uint8_t _Reserved;
    uint8_t Signature;
    uint32_t VolumeId;                  // serial number, value doesn't matter
    uint8_t VolumeLabel[11];
    uint8_t SystemId[8];

    // ... we don't care about code ...

} FAT_BootSector;

#endif