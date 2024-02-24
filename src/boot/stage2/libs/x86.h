#ifndef X86_H
#define X86_H

#include <stdint.h>
#include <netclover/bootutil.h>

#define ASM_FUNC __attribute__((cdecl))

void ASM_FUNC outb(uint16_t port, uint8_t value);
uint8_t ASM_FUNC inb(uint16_t port);

uint16_t ASM_FUNC diskReset16(int8_t drive);
// 0 if success, 1 if fail resetting
uint16_t ASM_FUNC diskRead16(int8_t drive, uint16_t cylinder, uint16_t head, uint16_t sector, uint8_t count, void* dataOut);
// 0 if success, 1 if fail reading, 2 if fail resetting
uint16_t ASM_FUNC diskGetDriveParams16(int8_t drive, uint8_t* driveTypeOut, uint16_t* cylindersOut, uint16_t* headsOut, uint16_t* sectorsOut);
// 0 if success, 1 if failed

void ASM_FUNC getVendorId(char* vendorOut);

int ASM_FUNC E820NextBlock(E820BlockStruct* block, uint32_t* contId);
#endif
