#include <stdint.h>
#include "libs/stdio.h"

const uint8_t bgcol = 0x80;
const uint8_t frcol = 0x3B;
const uint8_t txcol = 0x17;

const char message[][74] = {
    "Hello, my peeps! \x02                                                        ",
    "                                                                          ",
    "This is an operating system i've been working on this past week.          ",
    "For the uninitiated, an operating system is like MacOS or Windows.        ",
    "Hope y'all like it!                                                       ",
    "                                                                          ",
    "Its goal is to run a HTTP server and that's pretty much it.               ",
    "So far I only got this text displayed in the bootloader.                  ",
    "                                                                          ",
    "Also, by the way, this is running on bare metal hardware,                 ",
    "That is, no virtual machines (e.g. Virtualbox or QEMU) or black magic.    ",
    "                                                                          ",
    "If you're a nerd like me, check out the code below:                       ",
    "https://github.com/MachineThing/HTTP_OS                                   "
};

void putchrcol(int x, int y, char chr, uint8_t color) {
    putchr(x, y, chr);
    putcolor(x, y, color);
}

int cmain() {
    clrscr();
    setcursor(0, 17);
    for (int line = 0; line < 14; ++line) {
        putchrcol(0, line+1, 0xBA, frcol);  // 0xBA: ║
        putcolor(1, line+1, txcol);
        for (int chr = 0; chr < 74; ++chr) {
            putchrcol(chr+2, line+1, message[line][chr], txcol);
        }
        putchrcol(76, line+1, 0xBA, frcol); // 0xBA: ║
        putchrcol(77, line+1, ' ', bgcol);
    }

    for (int chr = 1; chr < 76; ++chr) {
        putchrcol(chr, 0, 0xCD, frcol);     // 0xC9: ═
        putchrcol(chr, 15, 0xCD, frcol);    // 0xC9: ═
        putchrcol(chr, 16, ' ', bgcol);
    }
    putchrcol(0, 16, ' ', bgcol);
    putchrcol(76, 16, ' ', bgcol);
    putchrcol(77, 16, ' ', bgcol);

    putchrcol(0, 0, 0xC9, frcol);           // 0xC9: ╔
    putchrcol(76, 0, 0xBB, frcol);          // 0xBB: ╗
    putchrcol(0, 15, 0xC8, frcol);          // 0xC8: ╚
    putchrcol(76, 15, 0xBC, frcol);         // 0xBC: ╝
    putchrcol(77, 15, ' ', bgcol);

    return 0;
}