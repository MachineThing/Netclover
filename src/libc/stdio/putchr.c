#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <netclover/io.h>
#include "vars.h"

void putchr(int x, int y, char chr) {
    screenBuffer[2*(y*SCREEN_WIDTH+x)] = chr;
}