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

    printf("Hello world from the kernel!\n");
    initKeyboard();
    for (;;) { asm("hlt"); };
    return 0;
}