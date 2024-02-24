#include <netclover/bootutil.h>
#include <stdio.h>
#include "memdetect.h"
#include "logger.h"
#include "x86.h"

const char memlogid[8] = "mapper";

void MemDetect(memInfoStruct* memInfo) {
    log(LOG_NORMAL, memlogid, "Getting memory map...");
    E820BlockStruct memRegions[MAX_REGIONS];
    E820BlockStruct block;
    uint32_t continuation = 0;
    int regionCount = 0;

    int ret = E820NextBlock(&block, &continuation);

    while (ret > 0 && continuation != 0) {
        memRegions[regionCount].Base = block.Base;
        memRegions[regionCount].Length = block.Length;
        memRegions[regionCount].Type = block.Type;
        memRegions[regionCount].ACPI = block.ACPI;

        regionCount++;
        
        log(LOG_NORMAL, memlogid, "E820: base=0x%llx | length=0x%llx | type=0x%x", block.Base, block.Length, block.Type);
        
        ret = E820NextBlock(&block, &continuation);
    }

    // Fill meminfo structure
    memInfo->RegionCount    = regionCount;
    memInfo->Regions        = memRegions;
}