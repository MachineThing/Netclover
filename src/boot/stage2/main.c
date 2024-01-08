#include <stdint.h>

const unsigned SCREEN_WIDTH = 80;
const unsigned SCREEN_HEIGHT = 25;
const uint8_t DEFAULT_COLOR = 0x7;

uint8_t* screenBuffer = (uint8_t*)0xB8000;
int screenX = 0, screenY = 0;

void print(char* str) {
    while(*str) {
        switch(*str) {
            case '\n':
                screenX = 0;
                screenY++;
                break;
            default:
                screenBuffer[2*(screenY*SCREEN_WIDTH+screenX)] = *str;
                screenX++;
                break;
        }
        str++;
    }
}

int cmain() {
    char message[] = "I love my doggo Pid!\nI\'m printing stuff with C!!!!\0";
    print(message);

    return 0;
}