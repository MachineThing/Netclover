#include <stdint.h>
#include <stdio.h>
#include <netclover/io.h>
#include "x86/gdt.h"
#include "x86/idt.h"
#include "drivers/pit/pit.h"

int main(uint16_t bootDrive) {
    initGDT();
    initIDT();
    clrscr();

    printf("Hello world from the kernel!");
    initPIT();
    for (;;);
    return 0;
}