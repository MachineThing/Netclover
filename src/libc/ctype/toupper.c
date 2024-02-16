#include <ctype.h>

char toupper(char c) {
    if (islower(c)) {
        return c - 'a' + 'A';
    } else {
        return c;
    }
}