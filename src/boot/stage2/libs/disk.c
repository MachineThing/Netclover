#include "disk.h"
#include "x86.h"
#include "stdio.h"
#include "logger.h"

const char disklogid[8] = "disk";

uint16_t DISK_Init(DISK* disk, uint8_t driveNumber) {
    uint8_t driveType;
    uint16_t cylinders, heads, sectors;

    if (diskGetDriveParams16(disk->id, &driveType, &cylinders, &heads, &sectors) != 0) {
        log(LOG_CRITICAL, disklogid, "Failed to init disk! Failed to get drive parameters.");
        return 1;
    }

    disk->id = driveNumber;
    disk->cylinders = cylinders;
    disk->heads = heads;
    disk->sectors = sectors;

    log(LOG_NORMAL, disklogid, "Disk geometry:");
    log(LOG_NORMAL, disklogid, "C: %i | H: %i | S: %i", cylinders, heads, sectors);

    return 0;
}

uint16_t DISK_Reset(DISK* disk) {
    if (diskReset16(disk->id) == 1) {
        log(LOG_ERROR, disklogid, "Failed to reset disk!");
        return 1;
    }
    return 0;
}

void DISK_LBA2CHS(DISK* disk, uint32_t lba, uint16_t* cylinderOut, uint16_t* headOut, uint16_t* sectorOut) {
    // Cylinders = (LBA / sectors_per_track) / heads
    *cylinderOut = (lba / disk->sectors) / disk->heads;

    // Heads = (LBA / sectors_per_track) % heads
    *headOut = (lba / disk->sectors) % disk->heads;

    // Sectors = (LBA % sectors_per_track + 1)
    *sectorOut = lba % disk->sectors + 1;
}

uint16_t DISK_ReadSectors(DISK* disk, uint32_t lba, uint8_t sectorsToRead, void* dataOut) {
    uint16_t cylinders, heads, sectors, result;

    DISK_LBA2CHS(disk, lba, &cylinders, &heads, &sectors);

    for (int i = 0; i < 3; i++) {
        log(LOG_NORMAL, disklogid, "Reading %i sectors from drive \"%i\" to 0x%x (attempt %i)", sectorsToRead, disk->id, dataOut, i+1);
        //log(LOG_NORMAL, disklogid, "LBA: %i", lba);
        //log(LOG_NORMAL, disklogid, "C: %i | H: %i | S: %i", cylinders, heads, sectors);
        
        result = diskRead16(disk->id, cylinders, heads, sectors, sectorsToRead, dataOut);

        if (result != 0) {
            char* error;

            switch (result) {
                case 0x01:  error = "Parameter out of range";                break;
                case 0x02:  error = "Address mark not found";                break;
                case 0x04:  error = "Sector not found/read error";           break;
                case 0x05:  error = "Reset failure";                         break;
                case 0x06:  error = "Disk changed";                          break;
                case 0x07:  error = "Drive parameter activity failed";       break;
                default:    error = "Unknown error"; 
            }
            log(LOG_ERROR, disklogid, "Failed to read disk! (%s)", error);
            if (DISK_Reset(disk) == (uint16_t)1) {
                return 1;
            }

        } else {
            return result;
        }
    }

    log(LOG_ERROR, disklogid, "Exhausted all three attempts.");
    return result;
}