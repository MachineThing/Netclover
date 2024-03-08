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

void initIDT() {
    idt_ptr.limit = sizeof(struct idt_entry_struct) * total_entries - 1;
    idt_ptr.offset = (uint64_t)&idt_entries;

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
    setIDTGate(0, isr0, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(1, isr1, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(2, isr2, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(3, isr3, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(4, isr4, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(5, isr5, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(6, isr6, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(7, isr7, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(8, isr8, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(9, isr9, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(10, isr10, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(11, isr11, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(12, isr12, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(13, isr13, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(14, isr14, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(15, isr15, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(16, isr16, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(17, isr17, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(18, isr18, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(19, isr19, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(20, isr20, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(21, isr21, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(22, isr22, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(23, isr23, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(24, isr24, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(25, isr25, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(26, isr26, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(27, isr27, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(28, isr28, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(29, isr29, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(30, isr30, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(31, isr31, 0x08, IDT_ATTRIBUTE_INTERRUPT);

    // IRQs
    setIDTGate(32, isq0, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(33, isq1, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(34, isq2, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(35, isq3, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(36, isq4, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(37, isq5, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(38, isq6, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(39, isq7, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(40, isq8, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(41, isq9, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(42, isq10, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(43, isq11, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(44, isq12, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(45, isq13, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(46, isq14, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(47, isq15, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    
    // System Calls
    setIDTGate(127, isr127, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    setIDTGate(128, isr128, 0x08, IDT_ATTRIBUTE_INTERRUPT);
    
    __asm__ __volatile__("lidt %0": :"g" (idt_ptr));
    __asm__ ("sti");
}

void setIDTGate(uint8_t num, void (void_offset)(), uint16_t selector, uint8_t attributes) {
    uint64_t offset = (uint64_t)void_offset;
    
    struct idt_entry_struct* entry = &idt_entries[num];

    entry->offset_low = (uint16_t)(offset & 0xFFFF);
    entry->offset_mid = (uint16_t)(offset >> 16);
    entry->offset_high = (uint32_t)(offset >> 32);
    entry->ist = 0;
    entry->selector = selector;
    entry->reserved = 0; // Always zero
    entry->attributes = attributes;
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

struct InterruptRegisters* isr_handler(struct InterruptRegisters* regs) {
    if (regs->int_no < 32) {
        printf("\nInterrupt! \"%s\" err: 0x%x\n", exception_messages[regs->int_no], regs->err_no);
        printf("RAX: 0x%x\n", regs->rax);
        printf("RBX: 0x%x\n", regs->rbx);
        printf("RCX: 0x%x\n", regs->rcx);
        printf("RDX: 0x%x\n", regs->rdx);
        //printf("RSI: 0x%x\n", regs->rsi);
        //printf("RDI: 0x%x\n", regs->rdi);
        //printf("RBP: 0x%x\n", regs->rbp);
        //printf("RSP: 0x%x\n", regs->rsp);
        printf("RIP: 0x%x\n", regs->rip);
    }
    
    return regs;
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