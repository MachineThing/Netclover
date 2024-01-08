#pragma once
#include <stdint.h>

#ifndef X86_H
#define X86_H

void __attribute__((cdecl)) outb(uint16_t port, uint8_t value);
uint8_t __attribute__((cdecl)) inb(uint16_t port);

#endif
