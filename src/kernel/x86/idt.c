#include <stdio.h>
#include <string.h>
#include <netclover/io.h>

#include "idt.h"

#define total_entries 256

#define MASTER_COMMS    0x20
#define SLAVE_COMMS     0xA0
#define MASTER_DATA     0x21
#define SLAVE_DATA      0xA1

struct idt_entry_struct idt_entries[total_entries];
struct idt_ptr_struct idt_ptr;

extern void idt_flush(uint32_t addr);

void initIDT() {
    idt_ptr.limit = sizeof(struct idt_entry_struct) * total_entries - 1;
    idt_ptr.offset = (uint32_t)(uintptr_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(struct idt_entry_struct) * total_entries);

    // Initialize Programmable Interrupt Controllers (PIC)
    uint8_t maskM = inb(MASTER_DATA);
    uint8_t maskS = inb(MASTER_DATA);

    outb(MASTER_COMMS, 0x11);   // Starts the initialization sequence
    outb(SLAVE_COMMS, 0x11);

    outb(MASTER_DATA, 0x20);    // Master PIC vector offset
    outb(SLAVE_DATA, 0x28);     // Slave PIC vector offset

    outb(MASTER_DATA, 0x04);    // Tell Master PIC there is a slave PIC at IRQ2
    outb(SLAVE_DATA, 0x02);     // Tell Slave PIC its cascade identity

    outb(MASTER_DATA, 0x01);    // Tell PICs to be in 8086 mode
    outb(SLAVE_DATA, 0x01);

    outb(MASTER_DATA, maskM);    // Restore saved masks
    outb(SLAVE_DATA, maskS);

    // Exceptions
    setIDTGate(0, (uint32_t)(uintptr_t)isr0, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(1, (uint32_t)(uintptr_t)isr1, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(2, (uint32_t)(uintptr_t)isr2, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(3, (uint32_t)(uintptr_t)isr3, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(4, (uint32_t)(uintptr_t)isr4, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(5, (uint32_t)(uintptr_t)isr5, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(6, (uint32_t)(uintptr_t)isr6, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(7, (uint32_t)(uintptr_t)isr7, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(8, (uint32_t)(uintptr_t)isr8, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(9, (uint32_t)(uintptr_t)isr9, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(10, (uint32_t)(uintptr_t)isr10, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(11, (uint32_t)(uintptr_t)isr11, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(12, (uint32_t)(uintptr_t)isr12, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(13, (uint32_t)(uintptr_t)isr13, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(14, (uint32_t)(uintptr_t)isr14, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(15, (uint32_t)(uintptr_t)isr15, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(16, (uint32_t)(uintptr_t)isr16, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(17, (uint32_t)(uintptr_t)isr17, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(18, (uint32_t)(uintptr_t)isr18, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(19, (uint32_t)(uintptr_t)isr19, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(20, (uint32_t)(uintptr_t)isr20, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(21, (uint32_t)(uintptr_t)isr21, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(22, (uint32_t)(uintptr_t)isr22, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(23, (uint32_t)(uintptr_t)isr23, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(24, (uint32_t)(uintptr_t)isr24, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(25, (uint32_t)(uintptr_t)isr25, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(26, (uint32_t)(uintptr_t)isr26, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(27, (uint32_t)(uintptr_t)isr27, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(28, (uint32_t)(uintptr_t)isr28, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(29, (uint32_t)(uintptr_t)isr29, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(30, (uint32_t)(uintptr_t)isr30, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(31, (uint32_t)(uintptr_t)isr31, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);

    // IRQs
    setIDTGate(32, (uint32_t)(uintptr_t)isq0, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(33, (uint32_t)(uintptr_t)isq1, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(34, (uint32_t)(uintptr_t)isq2, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(35, (uint32_t)(uintptr_t)isq3, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(36, (uint32_t)(uintptr_t)isq4, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(37, (uint32_t)(uintptr_t)isq5, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(38, (uint32_t)(uintptr_t)isq6, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(39, (uint32_t)(uintptr_t)isq7, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(40, (uint32_t)(uintptr_t)isq8, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(41, (uint32_t)(uintptr_t)isq9, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(42, (uint32_t)(uintptr_t)isq10, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(43, (uint32_t)(uintptr_t)isq11, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(44, (uint32_t)(uintptr_t)isq12, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(45, (uint32_t)(uintptr_t)isq13, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(46, (uint32_t)(uintptr_t)isq14, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(47, (uint32_t)(uintptr_t)isq15, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    
    // System Calls
    setIDTGate(127, (uint32_t)(uintptr_t)isr127, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    setIDTGate(128, (uint32_t)(uintptr_t)isr128, 0x08, IDT_ATTRIBUTE_INTERRUPT | IDT_ATTRIBUTE_RING0);
    
    idt_flush((uint32_t)(uintptr_t)&idt_ptr);
}

void setIDTGate(uint8_t num, uint32_t offset, uint16_t selector, uint8_t attributes) {
    idt_entries[num].offset_low = offset & 0xFFFF;
    idt_entries[num].offset_high = (offset >> 16) & 0xFFFF;
    idt_entries[num].selector = selector;
    idt_entries[num].reserved = 0; // Always zero
    idt_entries[num].attributes = attributes;
}

char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No x87 Coprocessor",
    "Double fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Fault",
    "Machine Check", 
    "SIMD Floating-Point Exception",
    "Virtualization",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

void isr_handler(struct InterruptRegisters* regs) {
    if (regs->int_no < 32) {
        printf("\nInterrupt! \"%s\" err: 0x%x\n", exception_messages[regs->int_no], regs->err_no);
        printf("Machine halted");
        for (;;);
    }
}

void* irq_routines[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters* regs)) {
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}

void irq_handler(struct InterruptRegisters* regs) {
    void (*handler)(struct InterruptRegisters *regs);

    handler = irq_routines[regs->int_no - 32];

    if (handler) {
        handler(regs);
    }

    if (regs->int_no >= 40) {
        outb(SLAVE_COMMS, 0x20);
    }

    outb(MASTER_COMMS, 0x20);
}