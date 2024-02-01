#ifndef STRING_H
#define STRING_H

char* strcpy(char *dest, const char *src);
const char* strchr(const char* str, char chr);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* dest, int value, size_t n);
int memcmp(const void* ptr1, const void* ptr2, size_t n);
size_t strlen(const char *str);

#endif
