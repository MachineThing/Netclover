#include <stdint.h>
#include <stdio.h>
#include <netclover/io.h>
#include <netclover/bootutil.h>
#include "x86/gdt.h"
#include "x86/idt.h"
#include "memory/pager.h"
#include "drivers/keyboard/keyboard.h"

int main(bootParamsStruct* bootParams) {
    clrscr();
    initGDT();
    initIDT();
    initPaging();

    printf("Boot device: 0x%x\n", bootParams->BootDevice);
    printf("%i regions\n", bootParams->Memory.RegionCount);
    for (int i=0; i<bootParams->Memory.RegionCount; i++) {
        printf("!");
    }

    printf("Hello world from the kernel!\n");
    initKeyboard();
    for (;;) { asm("hlt"); };
    return 0;
}