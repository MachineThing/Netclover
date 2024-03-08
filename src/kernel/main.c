#include <stdint.h>
#include <stdio.h>
#include <netclover/io.h>
#include <netclover/bootutil.h>
#include "x86/gdt.h"
#include "x86/idt.h"
#include "drivers/keyboard/keyboard.h"

int main(bootParamsStruct* bootParams) {
    clrscr();
    initGDT();
    initIDT();

    printf("Hello world from the kernel!\n");
    initKeyboard();
    for (;;) { asm("hlt"); };
    return 0;
}