#include <stdint.h>
#include "libs/stdio.h"

int main(uint16_t bootDrive) {
    clrscr();
    
    printf("Hello world from the kernel!");
    return 0;
}