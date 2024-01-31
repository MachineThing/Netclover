#include <stdarg.h>

#include "stdio.h"
#include "logger.h"

void log(enum LogLevel level, const char* service, const char* format, ...) {
    putc('[');
    for (int i = 0; i < 8; i++) {
        putc(service[i]);
    }
    puts("] ");
    switch (level) {
        case LOG_WARNING:
            puts("WARNING: ");
            break;
        case LOG_ERROR:
            puts("ERROR: ");
            break;
        case LOG_CRITICAL:
            puts("CRITICAL: ");
            break;
    }
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    putc('\n');
}