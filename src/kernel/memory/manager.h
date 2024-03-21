#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>
void kmalloc(size_t size);
void kfree(void* ptr);
void kmeminit();

#endif
