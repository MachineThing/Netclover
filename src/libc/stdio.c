#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include "netclover/io.h"

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

#define PRINTF_STATE_NORMAL         0
#define PRINTF_STATE_LENGTH         1
#define PRINTF_STATE_LENGTH_SHORT   2
#define PRINTF_STATE_LENGTH_LONG    3
#define PRINTF_STATE_SPEC           4

#define PRINTF_LENGTH_DEFAULT       0
#define PRINTF_LENGTH_SHORT_SHORT   1
#define PRINTF_LENGTH_SHORT         2
#define PRINTF_LENGTH_LONG          3
#define PRINTF_LENGTH_LONG_LONG     4

void printf_unsigned(unsigned long number, int radix) {
    char buffer[32];
    int pos = 0;
    do {
        unsigned long rem = number % radix;
        number /= radix;
        buffer[pos++] = "0123456789abcdef"[rem];
    } while (number > 0);

    while (--pos >= 0) {
        putc(buffer[pos]);
    }
}

void printf_signed(long number, int radix) {
    if (number < 0) {
        putc('-');
        printf_unsigned(-number, radix);
    } else printf_unsigned(number, radix);
}

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int state = PRINTF_STATE_NORMAL;
    int length = PRINTF_LENGTH_DEFAULT;
    int radix = 10;
    bool sign = false;
    bool number = false;

    while (*format) {
        switch (state) {
            case PRINTF_STATE_NORMAL:
                switch (*format) {
                    case '%':   state = PRINTF_STATE_LENGTH;
                                break;
                    default:    putc(*format);
                                break;
                }
                break;
            case PRINTF_STATE_LENGTH:
                switch (*format) {
                    case 'h':   length = PRINTF_LENGTH_SHORT;
                                state = PRINTF_STATE_LENGTH_SHORT;
                                break;
                    case 'l':   length = PRINTF_LENGTH_LONG;
                                state = PRINTF_STATE_LENGTH_LONG;
                                break;
                    default:    goto PRINTF_STATE_SPEC_;
                }
                break;
            case PRINTF_STATE_LENGTH_SHORT:
                if (*format == 'h') {
                    length = PRINTF_LENGTH_SHORT_SHORT;
                    state = PRINTF_STATE_SPEC;
                } else goto PRINTF_STATE_SPEC_;
                break;
            
            case PRINTF_STATE_LENGTH_LONG:
                if (*format == 'l') {
                    length = PRINTF_LENGTH_LONG_LONG;
                    state = PRINTF_STATE_SPEC;
                } else goto PRINTF_STATE_SPEC_;
                break;
            
            case PRINTF_STATE_SPEC:
            PRINTF_STATE_SPEC_:
                switch (*format) {
                    case 'c':   putc((char)va_arg(args, int));
                                break;
                    case 's':   puts(va_arg(args, const char*));
                                break;
                    case '%':   putc('%');
                                break;
                    case 'd':
                    case 'i':   radix = 10;
                                sign = true;
                                number = true;
                                break;
                    case 'u':   radix = 10;
                                sign = false;
                                number = true;
                                break;
                    case 'X':
                    case 'x':
                    case 'p':   radix = 16;
                                sign = false;
                                number = true;
                                break;
                    case 'o':   radix = 8;
                                sign = false;
                                number = true;
                                break;
                    // Ignore invalid spec
                    default:    break;
                }

                if (number) {
                    if (sign) {
                        switch (length) {
                            case PRINTF_LENGTH_SHORT_SHORT:
                            case PRINTF_LENGTH_SHORT:
                            case PRINTF_LENGTH_DEFAULT:     printf_signed(va_arg(args, int), radix);
                                                            break;

                            case PRINTF_LENGTH_LONG:        
                            case PRINTF_LENGTH_LONG_LONG:   printf_signed(va_arg(args, long), radix);
                                                            break;
                            
                        }
                    } else {
                        switch (length) {
                            case PRINTF_LENGTH_SHORT_SHORT:
                            case PRINTF_LENGTH_SHORT:
                            case PRINTF_LENGTH_DEFAULT:     printf_unsigned(va_arg(args, int), radix);
                                                            break;

                            case PRINTF_LENGTH_LONG:        
                            case PRINTF_LENGTH_LONG_LONG:   printf_unsigned(va_arg(args, long), radix);
                                                            break;
                        }
                    }
                }
                // Reset state
                state = PRINTF_STATE_NORMAL;
                length = PRINTF_LENGTH_DEFAULT;
                radix = 10;
                sign = false;
                break;
        }
        format++;
    }
    va_end(args);
}

void vprintf(const char* format, va_list args) {
    while(*format) {
        if (*format == '%' && (format + 1) != '\0') {
            switch (*(format+1)) {
                case 'c':   printf(va_arg(args, int));
                            break;
                case 's':   printf("%s", va_arg(args, char*));
                            break;
                case '%':   putc('%');
                            break;
                case 'd':
                case 'i':   printf("%d", va_arg(args, int));
                            break;
                case 'u':   printf("%u", va_arg(args, unsigned int));
                            break;
                case 'X':
                case 'x':
                case 'p':   printf("%p", va_arg(args, unsigned int));
                            break;
                case 'o':   printf("%o", va_arg(args, unsigned int));
                            break;
            }
            format += 2;
        } else {
            putc(*format);
            format++;
        }
    }
}