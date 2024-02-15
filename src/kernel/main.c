#include <stdint.h>
#include "libs/stdio.h"
#include "libs/io.h"
#include "x86/gdt.h"
#include "x86/idt.h"

int main(uint16_t bootDrive) {
    initGDT();
    initIDT();
    clrscr();

    printf("Hello world from the kernel!");
    return 0;
}