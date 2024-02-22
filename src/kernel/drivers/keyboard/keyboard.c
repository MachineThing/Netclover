#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <netclover/io.h>

#include "../../x86/idt.h"
#include "keyboard.h"

#define KEYBOARD_IN 0x60

Scancode scancodeMap[] = {
    {0x01, '\0'}, // ESC
    {0x02, '1'},
    {0x03, '2'},
    {0x04, '3'},
    {0x05, '4'},
    {0x06, '5'},
    {0x07, '6'},
    {0x08, '7'},
    {0x09, '8'},
    {0x0A, '9'},
    {0x0B, '0'},
    {0x0C, '-'},
    {0x0D, '='},
    {0x0E, '\b'}, // Backspace
    {0x0F, '\t'}, // Tab
    {0x10, 'q'},
    {0x11, 'w'},
    {0x12, 'e'},
    {0x13, 'r'},
    {0x14, 't'},
    {0x15, 'y'},
    {0x16, 'u'},
    {0x17, 'i'},
    {0x18, 'o'},
    {0x19, 'p'},
    {0x1A, '['},
    {0x1B, ']'},
    {0x1C, '\n'}, // Enter
    {0x1D, '\0'}, // Left Ctrl
    {0x1E, 'a'},
    {0x1F, 's'},
    {0x20, 'd'},
    {0x21, 'f'},
    {0x22, 'g'},
    {0x23, 'h'},
    {0x24, 'j'},
    {0x25, 'k'},
    {0x26, 'l'},
    {0x27, ';'},
    {0x28, '\''},
    {0x29, '`'},
    {0x2A, '\0'}, // Left Shift
    {0x2B, '\\'},
    {0x2C, 'z'},
    {0x2D, 'x'},
    {0x2E, 'c'},
    {0x2F, 'v'},
    {0x30, 'b'},
    {0x31, 'n'},
    {0x32, 'm'},
    {0x33, ','},
    {0x34, '.'},
    {0x35, '/'},
    {0x36, '\0'}, // Right Shift
    {0x37, '*'}, // Keypad *
    {0x38, '\0'}, // Left Alt
    {0x39, ' '}, // Space
    {0x3A, '\0'}, // Caps Lock
    {0x3B, '\0'}, // F1
    {0x3C, '\0'}, // F2
    {0x3D, '\0'}, // F3
    {0x3E, '\0'}, // F4
    {0x3F, '\0'}, // F5
    {0x40, '\0'}, // F6
    {0x41, '\0'}, // F7
    {0x42, '\0'}, // F8
    {0x43, '\0'}, // F9
    {0x44, '\0'}, // F10
    {0x45, '\0'}, // Num Lock
    {0x46, '\0'}, // Scroll Lock
    {0x47, '\0'}, // Home (7 on keypad)
    {0x48, '\0'}, // Up Arrow (8 on keypad)
    {0x49, '\0'}, // Page Up (9 on keypad)
    {0x4A, '-'}, // Keypad -
    {0x4B, '\0'}, // Left Arrow (4 on keypad)
    {0x4C, '\0'}, // Center (5 on keypad)
    {0x4D, '\0'}, // Right Arrow (6 on keypad)
    {0x4E, '+'}, // Keypad +
    {0x4F, '\0'}, // End (1 on keypad)
    {0x50, '\0'}, // Down Arrow (2 on keypad)
    {0x51, '\0'}, // Page Down (3 on keypad)
    {0x52, '\0'}, // Insert (0 on keypad)
    {0x53, '.'}, // Delete (. on keypad)
    {0x54, '\0'}, // Not used
    {0x55, '\0'}, // Not used
    {0x56, '\0'}, // Not used
    {0x57, '\0'}, // F11
    {0x58, '\0'}  // F12
};

Scancode numShiftMap[] = {
    {'`', '~'},
    {'1', '!'},
    {'2', '@'},
    {'3', '#'},
    {'4', '$'},
    {'5', '%'},
    {'6', '^'},
    {'7', '&'},
    {'8', '*'},
    {'9', '('},
    {'0', ')'},
    {'-', '_'},
    {'=', '+'},
};

bool capsOn;

void initKeyboard() {
    capsOn = false;
    irq_install_handler(1, &keyboardHandler);
}

void keyboardHandler(struct InterruptRegisters* regs) {
    char scanCode = inb(KEYBOARD_IN) & 0x7F;        // What key is pressed?
    bool pressed = (inb(KEYBOARD_IN) & 0x80) == 0;  // Pressed down or released?

    switch (scanCode) {
        case 0x2A:
        case 0x36:
            // Shift key
            capsOn = pressed;
            break;
        case 0x02 ... 0x0D:
        case 0x29:
            // Number keys
            if (pressed) {
                char ascii = getAscii(scanCode);
                if (capsOn) {
                    // Shift keys
                    for (int i = 0; i < sizeof(numShiftMap) / sizeof(numShiftMap[0]); ++i) {
                        if (numShiftMap[i].scanCode == ascii) {
                            putc(numShiftMap[i].ascii);
                        }
                    }
                } else {
                    // Regular keys
                    putc(ascii);
                }
            }
            break;
        case 0x10 ... 0x28:
        case 0x2B ... 0x35:
        case 0x39:
            if (pressed) {
                char ascii = getAscii(scanCode);
                if (capsOn) {
                    putc(ascii-32);
                } else {
                    putc(ascii);
                }
            }
            break;
        
        case 0x0E:
            if (pressed) {
                putc('\b');
            }
            break;
        default:
            break;
    }
}

char getAscii(char scanCode) {
    for (int i = 0; i < sizeof(scancodeMap) / sizeof(scancodeMap[0]); ++i) {
        if (scancodeMap[i].scanCode == scanCode) {
            return scancodeMap[i].ascii;
        }
    }
    return '\0'; // null if nothing
}