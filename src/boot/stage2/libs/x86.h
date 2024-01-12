#ifndef X86_H
#define X86_H

#include <stdint.h>

void __attribute__((cdecl)) outb(uint16_t port, uint8_t value);
uint8_t __attribute__((cdecl)) inb(uint16_t port);

void __attribute__((cdecl)) diskReset16(int8_t drive);
void __attribute__((cdecl)) diskRead16(int8_t drive, uint16_t cylinder, uint16_t head, uint16_t sector, uint8_t count, uint8_t* dataOut);
void __attribute__((cdecl)) diskGetDriveParams16(int8_t drive, uint8_t* driveTypeOut, uint16_t* cylindersOut, uint16_t* sectorsOut, uint16_t* headsOut);

void __attribute__((cdecl)) biosPrintTest(char* str);
#endif
