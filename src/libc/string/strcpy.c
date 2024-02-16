#include <string.h>

#include <stdint.h>
#include <stddef.h>

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