char isupper(char c) {
    return (c >= 'A' && c <= 'Z');
}

char islower(char c) {
    return (c >= 'a' && c <= 'z');
}

char toupper(char c) {
    if (islower(c)) {
        return c - 'a' + 'A';
    } else {
        return c;
    }
}

char tolower(char c) {
    if (isupper(c)) {
        return c - 'A' + 'a';
    } else {
        return c;
    }
}