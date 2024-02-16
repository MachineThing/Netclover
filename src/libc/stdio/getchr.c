#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <netclover/io.h>
#include "vars.h"

char getchr(int x, int y) {
    return screenBuffer[2 * (y * SCREEN_WIDTH + x)];
}