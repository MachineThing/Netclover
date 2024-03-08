#ifndef IDT_H
#define IDT_H

#include "stdint.h"

struct idt_entry_struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;            // Only first 2 bits are used
    uint8_t attributes;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;      // 0
} __attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit;
    uint64_t offset;
} __attribute__((packed));

struct InterruptRegisters {
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t int_no;
    uint64_t err_no;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
    // Register stack grows upward
} __attribute__((packed));

void initIDT();
void setIDTGate(uint8_t num, void (void_offset)(), uint16_t selector, uint8_t attributes);

struct InterruptRegisters* isr_handler(struct InterruptRegisters* regs);

void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters* regs));
void irq_uninstall_handler(int irq);
void irq_handler(struct InterruptRegisters* regs);

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

extern void isq0();
extern void isq1();
extern void isq2();
extern void isq3();
extern void isq4();
extern void isq5();
extern void isq6();
extern void isq7();
extern void isq8();
extern void isq9();
extern void isq10();
extern void isq11();
extern void isq12();
extern void isq13();
extern void isq14();
extern void isq15();

extern void isr127();
extern void isr128();

#endif