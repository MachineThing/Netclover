#include <ctype.h>

char tolower(char c) {
    if (isupper(c)) {
        return c - 'A' + 'a';
    } else {
        return c;
    }
}