#ifndef PIT_H
#define PIT_H

#include "../../x86/idt.h"

void initPIT();
void onIrq0(struct InterruptRegisters* regs);

#endif