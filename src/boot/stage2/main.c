#include <stdint.h>
#include "libs/stdio.h"
#include "libs/logger.h"
#include "libs/disk.h"
#include "libs/fat.h"

const char logid[8] = "stage2";

int cmain(uint16_t bootDrive) {
    clrscr();
    log(LOG_NORMAL, logid, "Loading kernel...");
    DISK disk;
    if (DISK_Init(&disk, bootDrive) == 0) {
        log(LOG_NORMAL, logid, "Initialized disk!");
    } else {
        log(LOG_CRITICAL, logid, "Failed to initialize disk.");
        while (true) {
        
        }
    }
    //asm("xchg %bx, %bx");
    FAT_Initialize(&disk);
    return 0;
}