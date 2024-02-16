#include <string.h>

#include <stdint.h>
#include <stddef.h>

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