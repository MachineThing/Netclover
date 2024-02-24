#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <netclover/io.h>

void vprintf(const char* format, va_list args) {
    while(*format) {
        if (*format == '%' && *(format+1) != '\0') {
            switch (*(format+1)) {
                case 'c':   printf("%c", va_arg(args, int));
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
                
                case 'l':   if (*(format+2)=='l') {
                                switch (*(format+3)) {
                                    case 'd':
                                    case 'i':   printf("%lld", va_arg(args, long long));
                                                break;
                                    case 'u':   printf("%llu", va_arg(args, unsigned long long));
                                                break;
                                    case 'X':
                                    case 'x':
                                    case 'p':   printf("%llp", va_arg(args, unsigned long long));
                                                break;
                                    case 'o':   printf("%llo", va_arg(args, unsigned long long));
                                                break;
                                }
                                format++;
                            } else {
                                switch (*(format+2)) {
                                    case 'd':
                                    case 'i':   printf("%ld", va_arg(args, long));
                                                break;
                                    case 'u':   printf("%lu", va_arg(args, unsigned long));
                                                break;
                                    case 'X':
                                    case 'x':
                                    case 'p':   printf("%lp", va_arg(args, unsigned long));
                                                break;
                                    case 'o':   printf("%lo", va_arg(args, unsigned long));
                                                break;
                                }
                            }
                            format++;
                            break;
                case 'h':   if (*(format+2)=='h') {
                                switch (*(format+3)) {
                                    case 'd':
                                    case 'i':   printf("%hhd", va_arg(args, int));
                                                break;
                                    case 'u':   printf("%hhu", va_arg(args, unsigned int));
                                                break;
                                    case 'X':
                                    case 'x':
                                    case 'p':   printf("%hhp", va_arg(args, unsigned int));
                                                break;
                                    case 'o':   printf("%hho", va_arg(args, unsigned int));
                                                break;
                                }
                                format++;
                            } else {
                                switch (*(format+2)) {
                                    case 'd':
                                    case 'i':   printf("%hd", va_arg(args, int));
                                                break;
                                    case 'u':   printf("%hu", va_arg(args, unsigned int));
                                                break;
                                    case 'X':
                                    case 'x':
                                    case 'p':   printf("%hp", va_arg(args, unsigned int));
                                                break;
                                    case 'o':   printf("%ho", va_arg(args, unsigned int));
                                                break;
                                }
                            }
                            format++;
                            break;
            }
            format += 2;
        } else {
            putc(*format);
            format++;
        }
    }
}