#include <stdint.h>
#include "x86.h"

const unsigned SCREEN_WIDTH = 80;
const unsigned SCREEN_HEIGHT = 25;
const uint8_t DEFAULT_COLOR = 0x7;

uint8_t* screenBuffer = (uint8_t*)0xB8000;
int screenX = 0, screenY = 0;

void putchr(int x, int y, char chr) {
    screenBuffer[2*(y*SCREEN_WIDTH+x)] = chr;
}

void putcolor(int x, int y, uint8_t color) {
    screenBuffer[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}

char getchr(int x, int y) {
    return screenBuffer[2 * (y * SCREEN_WIDTH + x)];
}

uint8_t getcolor(int x, int y) {
    return screenBuffer[2 * (y * SCREEN_WIDTH + x) + 1];
}

void setcursor(int x, int y) {
    int pos = y * SCREEN_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void clrscr() {
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putchr(x, y, '\0');
            putcolor(x, y, DEFAULT_COLOR);
        }

    screenX = 0;
    screenY = 0;
    setcursor(screenX, screenY);
}

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

void puts(const char* str) {
    while(*str) {
        putc(*str);
        str++;
    }
}