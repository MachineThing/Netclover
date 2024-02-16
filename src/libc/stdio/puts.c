#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <netclover/io.h>

void puts(const char* str) {
    while(*str) {
        putc(*str);
        str++;
    }
}