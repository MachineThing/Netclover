#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <netclover/io.h>

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

#if !defined(__x86_64__)
// 32 Bit support for long long division
unsigned long long __udivdi3(unsigned long long a, unsigned long long b) {
    unsigned long long quotient = 0;
    unsigned long long remainder = 0;
    unsigned long long divisor = b;

    // Perform long division
    for (int i = 63; i >= 0; i--) {
        remainder <<= 1;
        remainder |= (a >> i) & 1;
        if (remainder >= divisor) {
            remainder -= divisor;
            quotient |= (1ULL << i);
        }
    }

    return quotient;
}

unsigned long long __umoddi3(unsigned long long a, unsigned long long b) {
    unsigned long long remainder = 0;
    unsigned long long divisor = b;

    // Perform long division
    for (int i = 63; i >= 0; i--) {
        remainder <<= 1;
        remainder |= (a >> i) & 1;
        if (remainder >= divisor) {
            remainder -= divisor;
        }
    }

    return remainder;
}
#endif

void printf_unsigned(unsigned long long number, int radix) {
    char buffer[32];
    int pos = 0;
    do {
        unsigned long long rem = number % radix;
        number /= radix;
        buffer[pos++] = "0123456789abcdef"[rem];
    } while (number > 0);

    while (--pos >= 0) {
        putc(buffer[pos]);
    }
}

void printf_signed(long long number, int radix) {
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

                            case PRINTF_LENGTH_LONG:        printf_signed(va_arg(args, long), radix);
                                                            break;

                            case PRINTF_LENGTH_LONG_LONG:   printf_signed(va_arg(args, long long), radix);
                                                            break;
                            
                        }
                    } else {
                        switch (length) {
                            case PRINTF_LENGTH_SHORT_SHORT:
                            case PRINTF_LENGTH_SHORT:
                            case PRINTF_LENGTH_DEFAULT:     printf_unsigned(va_arg(args, unsigned int), radix);
                                                            break;

                            case PRINTF_LENGTH_LONG:        printf_unsigned(va_arg(args, unsigned long), radix);
                                                            break;
                                                            
                            case PRINTF_LENGTH_LONG_LONG:   printf_unsigned(va_arg(args, unsigned long long), radix);
                                                            break;
                        }
                    }
                }
                // Reset state
                state = PRINTF_STATE_NORMAL;
                length = PRINTF_LENGTH_DEFAULT;
                radix = 10;
                sign = false;
                number = false;
                break;
        }
        format++;
    }
    va_end(args);
}