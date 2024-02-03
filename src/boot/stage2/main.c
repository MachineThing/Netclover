#include <stdint.h>
#include "libs/stdio.h"
#include "libs/logger.h"
#include "libs/disk.h"
#include "libs/fat.h"
#include "libs/x86.h"
#include "libs/memdefs.h"

const char logid[8] = "stage2";

int cmain(uint16_t bootDrive) {
    clrscr();
    static char vendorid[13] = ""; // CPU ID is 12 bytes and null pointer = 13 bytes
    
    getVendorId(vendorid);
    log(LOG_NORMAL, logid, "Venderid: %s", vendorid);

    log(LOG_NORMAL, logid, "Loading kernel...");
    DISK disk;
    if (DISK_Init(&disk, bootDrive) == 0) {
        log(LOG_NORMAL, logid, "Initialized disk!");
    } else {
        log(LOG_CRITICAL, logid, "Failed to initialize disk.");
        return 1;
    }
    /*If vender id is QEMU or Bochs then enable debug info*/

    //asm("xchg %bx, %bx");
    if (FAT_Initialize(&disk) == 0) {
        log(LOG_NORMAL, logid, "Initialized FAT!");
    } else {
        log(LOG_CRITICAL, logid, "Failed to initialize FAT.");
        return 1;
    }

    FAT_File* kernel = FAT_Open(&disk, "/kernel.bin");
    uint32_t read;
    uint8_t* kernelBuffer = (uint8_t*)MEMORY_KERNEL_ADDR;

    log(LOG_NORMAL, logid, "At this point the kernel should be able to be read!");

    return 0;
}