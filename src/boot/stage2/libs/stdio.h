#ifndef STDIO_H
#define STDIO_H

#include <stdint.h>

void putchr(int x, int y, char chr);
void putcolor(int x, int y, uint8_t color);

char getchr(int x, int y);
uint8_t getcolor(int x, int y);

void setcursor(int x, int y);
void clrscr();
void scrollback(int lines);

void putc(char chr);
void puts(const char* str);

#endif
