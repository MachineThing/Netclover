#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../x86/idt.h"

void initKeyboard();
void keyboardHandler(struct InterruptRegisters* regs);
char getAscii(char scanCode);

typedef struct {
    uint8_t scanCode;
    char ascii;
} Scancode;

#endif