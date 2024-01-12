#include <stdint.h>
#include "libs/stdio.h"
#include "libs/x86.h"

int cmain() {
    char* message = "[boot] This is from real mode\n\r\0";
    clrscr();
    puts("[boot] Loading kernel...\n");
    biosPrintTest(message);
    puts("[boot] This is from protected mode\n");
    return 0;
}