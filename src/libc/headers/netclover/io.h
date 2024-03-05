#ifndef X86_H
#define X86_H

#include <stdint.h>

#if defined(__x86_64__)
// 64 Bit
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
#else
// 32 Bit
void __attribute__((cdecl)) outb(uint16_t port, uint8_t value);
uint8_t __attribute__((cdecl)) inb(uint16_t port);
#endif

#endif
