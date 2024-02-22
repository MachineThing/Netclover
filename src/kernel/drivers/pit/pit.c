#include <stdint.h>
#include <stdio.h>
#include <netclover/io.h>

#include "../../x86/idt.h"
#include "pit.h"

uint64_t ticks;
const uint32_t freq = 10;

#define PIT_CH0     0x40
#define PIT_CH1     0x41
#define PIT_CH2     0x42
#define PIT_COMMS   0x43

void initPIT() {
    ticks = 0;
    irq_install_handler(0, &onIrq0);

    // The oscillator is at 1.1931816666 MHz
    uint32_t divisor = 1191380/freq;

    outb(PIT_COMMS, 0x36); // Channel 0, lobyte/hibyte square wave
    outb(PIT_CH0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CH0, (uint8_t)((divisor >> 8) & 0xFF));
}

void onIrq0(struct InterruptRegisters* regs) {
    ticks++;
    printf("Tick! (%d)\n", ticks);
}