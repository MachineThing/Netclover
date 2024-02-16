#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <netclover/io.h>
#include "vars.h"

void putcolor(int x, int y, uint8_t color) {
    screenBuffer[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}