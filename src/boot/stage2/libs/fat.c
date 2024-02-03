#include <stddef.h>

#include "fat.h"
#include "stdio.h"
#include "ctype.h"
#include "logger.h"
#include "string.h"
#include "memdefs.h"
#include "minmax.h"

const char fatlogid[8] = "filesys";

#define SECTOR_SIZE             512
#define MAX_PATH_SIZE           256
#define MAX_FILE_HANDLES        10
#define ROOT_DIRECTORY_HANDLE   -1

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

    uint32_t rootDirSectors = (rootDirSize + bootSector.BytesPerSector - 1) / bootSector.BytesPerSector;
    dataSectionLba = rootDirLba + rootDirSectors;

    for (int i = 0; i < MAX_FILE_HANDLES; i++) {
        fatData->OpenedFiles[i].Opened = false;
    }
    return 0;
}

uint32_t FAT_ClusterToLba(uint32_t cluster) {
    return dataSectionLba + (cluster - 2) * bootSector.SectorsPerCluster;
}

FAT_File* FAT_OpenEntry(DISK* disk, FAT_DirectoryEntry* entry) {
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
    FAT_FileData* fd = &fatData->OpenedFiles[handle];
    fd->Public.Handle = handle;
    fd->Public.IsDirectory = (entry->Attributes & FAT_ATTRIBUTE_DIRECTORY != 0);
    fd->Public.Position = 0;
    fd->Public.Size = entry->Size;
    fd->FirstCluster = entry->FirstClusterLow + ((uint32_t)entry->FirstClusterHigh << 16);
    fd->CurrentCluster = fd->FirstCluster;
    fd->CurrentSectorInCluster = 0;

    if (DISK_ReadSectors(disk, FAT_ClusterToLba(fd->CurrentCluster), 1, fd->Buffer) != 0) {
        log(LOG_ERROR, fatlogid, "Open entry failed. Cluster=%u LBA=%u", fd->CurrentCluster, FAT_ClusterToLba(fd->CurrentCluster));
        for (int i = 0; i < 11; i++) {
            printf("%c", entry->Name[i]);
        }
        putc('\n');
        return (FAT_File*)NULL;
    }

    fd->Opened = true;
    return &fd->Public;
}

FAT_File* FAT_Open(DISK* disk, const char* path) {
    char name[MAX_PATH_SIZE];

    if (path[0] == '/') { // Ignore leading slash
        path++;
    }

    FAT_File* current = &fatData->RootDirectory.Public;

    while (*path) {
        // Extract next file name from path
        bool isLast = false;
        const char* delim = strchr(path, '/');
        if (delim != NULL) {
            memcpy(name, path, delim - path);
            name[delim - path + 1] = '\0';
            path = delim + 1;
        } else {
            unsigned len = strlen(path);
            memcpy(name, path, len);
            name[len + 1] = '\0';
            path += len;
            isLast = true;
        }
        // Find directory entry in current directory
        FAT_DirectoryEntry entry;
        bool canFindFile = FAT_FindFile(disk, current, name, &entry);
        FAT_Close(current);
        if (canFindFile) {
            if (!isLast && entry.Attributes & FAT_ATTRIBUTE_DIRECTORY == 0) {
                log(LOG_ERROR, fatlogid, "%s is not a directory", name);
                return NULL;
            }

            log(LOG_NORMAL, fatlogid, "Opening %s", name);
            current = FAT_OpenEntry(disk, &entry);
        } else {
            log(LOG_ERROR, fatlogid, "file \"%s\" not found", name);
            return NULL;
        }
    }

    return current;
}

bool FAT_FindFile(DISK* disk, FAT_File* file, const char* name, FAT_DirectoryEntry* entryOut) {
    char fatName[12];
    FAT_DirectoryEntry entry;

    memset(fatName, ' ', sizeof(fatName));
    fatName[11] = '\0';

    const char* ext = strchr(name, '.');
    if (ext == NULL) {
        ext = name + 11;
    }

    for (int i = 0; i < 8 && name[i] && name + i < ext; i++) {
        fatName[i] = toupper(name[i]);
    }

    if (ext != name + 11) {
        for (int i = 0; i < 3 && ext[i + 1]; i++) {
            fatName[i + 8] = toupper(ext[i + 1]);
        }
    }

    while (FAT_ReadEntry(disk, file, &entry)) {
        if (memcmp(fatName, entry.Name, 11) == 0) {
            *entryOut = entry;
            return true;
        }
    }
    return false;
}

uint32_t FAT_Read(DISK* disk, FAT_File* file, uint32_t byteCount, void* dataOut) {
    // Get file data
    FAT_FileData* fd = (file->Handle == ROOT_DIRECTORY_HANDLE)
        ? &fatData->RootDirectory
        : &fatData->OpenedFiles[file->Handle];
    
    uint8_t* u8Dataout = (uint8_t*)dataOut;

    if (!fd->Public.IsDirectory || (fd->Public.IsDirectory && fd->Public.Size != 0)) {
        byteCount = min(byteCount, fd->Public.Size - fd->Public.Position);
    }

    while (byteCount > 0) {
        uint32_t leftInBuffer = SECTOR_SIZE - (fd->Public.Position % SECTOR_SIZE);
        uint32_t take = min(byteCount, leftInBuffer);

        memcpy(u8Dataout, fd->Buffer + fd->Public.Position % SECTOR_SIZE, take);
        u8Dataout += take;
        fd->Public.Position += take;
        byteCount -= take;

        if (leftInBuffer == take) {
            if (fd->Public.Handle == ROOT_DIRECTORY_HANDLE) {
                ++fd->CurrentCluster;
                if (DISK_ReadSectors(disk, fd->CurrentCluster, 1, fd->Buffer) != 0) {
                    log(LOG_ERROR, fatlogid, "Read error on take!");
                    break;
                } 
            } else {
                if (++fd->CurrentSectorInCluster >= bootSector.SectorsPerCluster) {
                    fd->CurrentSectorInCluster = 0;
                    fd->CurrentCluster = FAT_NextCluster(fd->CurrentCluster);
                }

                if (fd->CurrentCluster >= 0xFF8) {
                    // End of file
                    fd->Public.Size = fd->Public.Position;
                    break;
                }

                // Read next sector
                asm("xchg %bx, %bx");
                if (DISK_ReadSectors(disk, FAT_ClusterToLba(fd->CurrentCluster) + fd->CurrentSectorInCluster, 1, fd->Buffer) != 0) {
                    log(LOG_ERROR, fatlogid, "Read error!");
                    break;
                }
            }
        }
    }

    return u8Dataout - (uint8_t*)dataOut;
}

bool FAT_ReadEntry(DISK* disk, FAT_File* file, FAT_DirectoryEntry* dirEntry) {
    return FAT_Read(disk, file, sizeof(FAT_DirectoryEntry), dirEntry) == sizeof(FAT_DirectoryEntry);
}

uint32_t FAT_NextCluster(uint32_t currentCluster) {
    uint32_t fatIndex = currentCluster * 3 / 2;

    if (currentCluster % 2 == 0)
        return (*(uint16_t*)(fat + fatIndex)) & 0x0FFF;
    else
        return (*(uint16_t*)(fat + fatIndex)) >> 4;
}

void FAT_Close(FAT_File* file) {
    if (file->Handle == ROOT_DIRECTORY_HANDLE) {
        file->Position = 0;
        rootDirectory.CurrentCluster = fatData->RootDirectory.FirstCluster;
    } else {
        fatData->OpenedFiles[file->Handle].Opened = false;
    }
}
