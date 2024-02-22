#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <netclover/io.h>
#include "vars.h"

void scrollback(int lines) {
    for (int y = lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            putchr(x, y - lines, getchr(x, y));
            putcolor(x, y - lines, getcolor(x, y));
        }

    for (int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            putchr(x, y, '\0');
            putcolor(x, y, DEFAULT_COLOR);
        }

    screenY -= lines;
}

void putc(char chr) {
    switch(chr) {
        case '\n':
            screenX = 0;
            screenY++;
            break;

        case '\r':
            screenX = 0;
            break;

        case '\t':
            for (int i = 0; i < 4 - (screenX % 4); i++)
                putc(' ');
            break;

        case '\b':
            if (screenX == 0) {
                screenX = SCREEN_WIDTH-1;
                screenY--;
            } else {
                screenX--;
            }
            putchr(screenX, screenY, ' ');
            break;
        default:
            putchr(screenX, screenY, chr);
            screenX++;
            break;
    }
    if (screenX >= SCREEN_WIDTH)
    {
        screenY++;
        screenX = 0;
    }
    if (screenY >= SCREEN_HEIGHT)
        scrollback(1);

    setcursor(screenX, screenY);
}