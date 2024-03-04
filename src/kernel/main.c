#include <stdint.h>
#include <stdio.h>
#include <netclover/io.h>
#include <netclover/bootutil.h>
#include "x86/gdt.h"
#include "x86/idt.h"
#include "drivers/keyboard/keyboard.h"

int main(bootParamsStruct* bootParams) {
    initGDT();
    initIDT();
    clrscr();

    printf("Hello world from the kernel! Boot drive is 0x%x\n", bootParams->BootDevice);
    initKeyboard();
    for (;;);
    return 0;
}