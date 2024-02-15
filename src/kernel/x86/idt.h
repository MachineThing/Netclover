#ifndef IDT_H
#define IDT_H

#include "stdint.h"

struct idt_entry_struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit;
    uint32_t offset;
} __attribute__((packed));

struct InterruptRegisters {
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;
    uint32_t err_no;
    uint32_t eip;
    uint32_t csm;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
} __attribute__((packed));

void initIDT();
void setIDTGate(uint8_t num, uint32_t offset, uint16_t selector, uint8_t attributes);

void isr_handler(struct InterruptRegisters* regs);

// Note: bit 8 is set for present bit

#define IDT_ATTRIBUTE_TASK      0x85 // 1000 0101
#define IDT_ATTRIBUTE_INTERRUPT 0x8E // 1000 1110
#define IDT_ATTRIBUTE_TRAP      0x8F // 1000 1111

#define IDT_ATTRIBUTE_RING0     0x00 // 0000 0000
#define IDT_ATTRIBUTE_RING1     0x20 // 0010 0000
#define IDT_ATTRIBUTE_RING2     0x40 // 0100 0000
#define IDT_ATTRIBUTE_RING3     0x60 // 0110 0000

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void isr127();
extern void isr128();

#endif