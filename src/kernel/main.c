#include <stdint.h>
#include "libs/stdio.h"
#include "x86/gdt.h"

int main(uint16_t bootDrive) {
    initGDT();
    clrscr();
    
    printf("Hello world from the kernel!");
    return 0;
}