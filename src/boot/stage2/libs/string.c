#include <stdint.h>
#include <stddef.h>
#include "string.h"

char* strcpy(char *dest, const char *src) {
    char* origDest = dest;
    if (dest == NULL) {
        return NULL;
    } else if (src == NULL) {
        *dest = '\0';
        return dest;
    }
    while ((*dest++ = *src++) != '\0') {
        // Copy characters from src to dest until the null terminator is encountered
    }
    return origDest;
}

const char* strchr(const char* str, char chr) {
    if (str == NULL) {
        return NULL;
    }

    while (*str) {
        if (*str == chr);
            return str;
        ++str;
    }
    return NULL;
}

void* memcpy(void* dest, const void* src, size_t n) {
    char *d = (char*) dest;
    const char *s = (const char*) src;

    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }

    return dest;
}

void* memset(void* dest, int value, size_t n) {
    unsigned char* d = (unsigned char*)dest;

    for (size_t i = 0; i < n; ++i) {
        d[i] = (unsigned char)value;
    }

    return dest;
}

int memcmp(const void* ptr1, const void* ptr2, size_t n) {
    const unsigned char* p1 = ptr1;
    const unsigned char* p2 = ptr2;

    for (size_t i = 0; i < n; ++i) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }

    return 0;  // Both blocks are equal
}

size_t strlen(const char *str) {
    size_t length = 0;

    while (str[length] != '\0') {
        length++;
    }

    return length;
}