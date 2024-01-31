#include "fat.h"
#include "stdio.h"
#include "logger.h"
#include "string.h"
#include "memdefs.h"

const char fatlogid[8] = "filesys";

#define SECTOR_SIZE             512
#define MAX_PATH_SIZE           256
#define MAX_FILE_HANDLES        10
#define ROOT_DIRECTORY_HANDLE   -1
#define MEMORY_FAT_SIZE         0x10000

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

} __attribute__((packed)) FAT_BootSector;

typedef struct
{
    uint8_t Buffer[SECTOR_SIZE];
    FAT_File Public;
    bool Opened;
    uint32_t FirstCluster;
    uint32_t CurrentCluster;
    uint32_t CurrentSectorInCluster;

} FAT_FileData;

typedef struct
{
    union
    {
        FAT_BootSector BootSector;
        uint8_t BootSectorBytes[SECTOR_SIZE];
    } BS;

    FAT_FileData RootDirectory;

    FAT_FileData OpenedFiles[MAX_FILE_HANDLES];
} FAT_Data;

static FAT_Data* data;

uint16_t FAT_Initialize(DISK* disk) {
    data = (FAT_Data*)MEMORY_FAT_ADDR;
    if (DISK_ReadSectors(disk, 0, 1, data->BS.BootSectorBytes) != 0) {
        log(LOG_ERROR, fatlogid, "Failed to read bootsector!");
        return 1;
    } else {
        return 0;
    }
}

FAT_File* FAT_Open(DISK* disk, const char* path) {

}

uint32_t FAT_Read(DISK* disk, FAT_File file, uint32_t byteCount, void* dataOut) {

}

uint16_t FAT_ReadEntry(DISK* disk, FAT_File file, FAT_DirectoryEntry* dirEntry) {

}

void FAT_Close(FAT_File* file) {

}
