#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <netclover/io.h>

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