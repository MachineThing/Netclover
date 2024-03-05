#ifndef GDT_H
#define GDT_H

#include "stdint.h"

struct gdt_entry_struct {
    // https://wiki.osdev.org/GDT#Segment_Descriptor

    uint16_t        limit;      // 16 bits limit
    uint16_t        base_low;   // 16 bits base
    uint8_t         base_mid;   // 8 bits base
    uint8_t         access;     // 8 bits access byte
    uint8_t         flags;      // 4 bits limit (16+4   = 20 bits total)
                                // 4 bits flags
    uint8_t         base_high;  // 8 bits base  (16+8+8 = 32 bits total)
} __attribute__((packed));

struct gdt_ptr_struct {
    uint16_t        limit;
    uint32_t        base;
} __attribute__((packed));

struct tss_entry_struct {
    // https://wiki.osdev.org/Task_State_Segment#Protected_Mode
    uint32_t link;      // Previous Task Link Field
    // Segment Selectors and Stack Pointers
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;

    // Special Registers
    uint32_t eip;       // Instruction Pointer
    uint32_t eflags;    // EFLAGS Register

    // General Purpose Registers
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;

    // Segment Registers
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;

    // Special Registers
    uint32_t ldtr;      // Local Descriptor Table Pointer
    uint32_t iopb;      // I/O Map Base Address Field
    uint32_t ssp;       // Shadow Stack Pointer
} __attribute__((packed));

#define GDT_ACCESS_PRESENT  0x80 // 1000 0000
#define GDT_ACCESS_RING0    0x00 // 0000 0000
#define GDT_ACCESS_RING1    0x20 // 0010 0000
#define GDT_ACCESS_RING2    0x40 // 0100 0000
#define GDT_ACCESS_RING3    0x60 // 0110 0000
#define GDT_ACCESS_TYPE     0x10 // 0001 0000
#define GDT_ACCESS_EXEC     0x08 // 0000 1000
#define GDT_ACCESS_CONF     0x04 // 0000 0100
#define GDT_ACCESS_RW       0x02 // 0000 0010
#define GDT_ACCESS_ACC      0x01 // 0000 0001

void initGDT();
void setGDTGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void writeTSS(uint32_t num, uint16_t ss0, uint32_t esp0);

#endif