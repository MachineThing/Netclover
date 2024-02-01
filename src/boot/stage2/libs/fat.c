#include <stddef.h>

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

static FAT_Data* fatData;
static uint8_t* fat = NULL;
static uint32_t dataSectionLba;

#define bootSector fatData->BS.BootSector
#define rootDirectory fatData->RootDirectory

uint16_t FAT_Initialize(DISK* disk) {
    fatData = (FAT_Data*)MEMORY_FAT_ADDR;
    // Read boot sector
    if (DISK_ReadSectors(disk, 0, 1, fatData->BS.BootSectorBytes) != 0) {
        log(LOG_ERROR, fatlogid, "Failed to read bootsector!");
        return 1;
    }
    // Check signature
    if (bootSector.Signature != 0x29) {
        log(LOG_ERROR, fatlogid, "Bad bootsector signature!");
        return 1;
    }
    // Get FAT Size
    fat = (uint8_t*)fatData + sizeof(FAT_Data);
    uint32_t fatSize = bootSector.BytesPerSector * bootSector.SectorsPerFat;
    if (sizeof(FAT_Data) + fatSize >= MEMORY_FAT_SIZE) {
        log(LOG_ERROR, fatlogid, "Not enough memory to read FAT! (Required %u but only have %u)", sizeof(FAT_Data) + fatSize, MEMORY_FAT_SIZE);
        return 1;
    }
    // Read FAT
    if (DISK_ReadSectors(disk, bootSector.ReservedSectors, bootSector.SectorsPerFat, fat) != 0) {
        log(LOG_ERROR, fatlogid, "Failed to read FAT!");
        return 1;
    }
    // Read root directory
    uint32_t rootDirLba = bootSector.ReservedSectors + bootSector.SectorsPerFat * bootSector.FatCount;
    uint32_t rootDirSize = sizeof(FAT_DirectoryEntry) * bootSector.DirEntryCount;

    rootDirectory.Public.Handle = ROOT_DIRECTORY_HANDLE;
    rootDirectory.Public.IsDirectory = true;
    rootDirectory.Public.Position = 0;
    rootDirectory.Public.Size = sizeof(FAT_DirectoryEntry) * bootSector.DirEntryCount;
    rootDirectory.Opened = true;
    rootDirectory.FirstCluster = rootDirLba;
    rootDirectory.CurrentCluster = rootDirLba;
    rootDirectory.CurrentSectorInCluster = 0;

    if (DISK_ReadSectors(disk, rootDirLba, 1, rootDirectory.Buffer)) {
        log(LOG_ERROR, fatlogid, "Failed to read root!");
        return 1;
    }

    uint32_t rootDirSectors = (rootDirSize * bootSector.BytesPerSector - 1) / bootSector.BytesPerSector;
    dataSectionLba = rootDirLba + rootDirSectors;

    for (int i = 0; i < MAX_FILE_HANDLES; i++) {
        fatData->OpenedFiles[i].Opened = false;
    }
    return 0;
}

FAT_File* FAT_Open(DISK* disk, FAT_DirectoryEntry* entry) {
    int handle = -1;
    for (int i = 0; i < MAX_FILE_HANDLES && handle < 0; i++) {
        if (!fatData->OpenedFiles[i].Opened) {
            handle = i;
        }
    }
    
    // Out of handles?
    if (handle < 0) {
        log(LOG_ERROR, fatlogid, "No more file handles!");
        return (FAT_File*)NULL;
    }

    // Setup vars
    return (FAT_File*)NULL;
}

uint32_t FAT_Read(DISK* disk, FAT_File file, uint32_t byteCount, void* dataOut) {

}

uint16_t FAT_ReadEntry(DISK* disk, FAT_File file, FAT_DirectoryEntry* dirEntry) {

}

void FAT_Close(FAT_File* file) {

}
