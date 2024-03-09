#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <netclover/io.h>
#include "vars.h"
#if !defined(__x86_64__)
// If in bootloader
const unsigned SCREEN_WIDTH = 80;
const unsigned SCREEN_HEIGHT = 24;
#else
const unsigned SCREEN_WIDTH = 80;
const unsigned SCREEN_HEIGHT = 25;
#endif
const uint8_t DEFAULT_COLOR = 0x7;

uint8_t* screenBuffer = (uint8_t*)0xB8000;
int screenX = 0;
int screenY = 0;