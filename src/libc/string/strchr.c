#include <string.h>

#include <stdint.h>
#include <stddef.h>

const char* strchr(const char* str, char chr) {
    if (str == NULL) {
        return NULL;
    }

    while (*str) {
        if (*str == chr) {
            return str;
        }
        ++str;
    }
    return NULL;
}