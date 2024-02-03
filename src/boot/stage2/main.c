#include <stdint.h>
#include "libs/stdio.h"
#include "libs/logger.h"
#include "libs/disk.h"
#include "libs/fat.h"
#include "libs/x86.h"
#include "libs/memdefs.h"
#include "libs/string.h"

const char logid[8] = "stage2";

typedef void (*kernelExe)();
uint8_t* memLoadBuffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* memKernel = (uint8_t*)MEMORY_KERNEL_ADDR;

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

    FAT_File* kernelfd = FAT_Open(&disk, "/kernel.bin");
    uint32_t read;
    uint8_t* memBuffer = memKernel;

    while ((read = FAT_Read(&disk, kernelfd, MEMORY_LOAD_SIZE, memLoadBuffer))) {
        memcpy(memBuffer, memLoadBuffer, read);
        memBuffer += read;
    }
    
    FAT_Close(kernelfd);
    kernelExe kernelStart = (kernelExe)memKernel;
    
    kernelStart(bootDrive);

    return 0;
}