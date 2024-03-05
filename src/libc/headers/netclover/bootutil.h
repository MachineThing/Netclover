#ifndef BOOTUTIL_H
#define BOOTUTIL_H

#include <stdint.h>

#define MAX_E820_REGIONS 256

typedef struct {
    uint64_t Base;
    uint64_t Length;
    uint32_t Type;
    uint32_t ACPI;
} E820BlockStruct;

enum E820MemBlockType {
    E820_SYSMEM         = 1,
    E820_RESERVED       = 2,
    E820_ACPI_RECLAIM   = 3,
    E820_ACPI_NVS       = 4,
    E820_BADMEM         = 5
};

typedef struct {
    int RegionCount;
    E820BlockStruct* Regions;
} memInfoStruct;

typedef struct {
    memInfoStruct Memory;
    uint8_t BootDevice;
} bootParamsStruct;

#endif
