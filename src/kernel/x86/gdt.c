#include <string.h>
#include "gdt.h"

extern void gdt_flush(uint32_t addr);
extern void tss_flush();

#define total_entries 6

struct gdt_entry_struct gdt_entries[total_entries];
struct gdt_ptr_struct   gdt_ptr;
struct tss_entry_struct tss_entry;

void initGDT() {
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * total_entries) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    setGDTGate(0, 0, 0, 0, 0); // Null segment

    setGDTGate(1, 0, 0xFFFFFFFF,
    GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_TYPE | GDT_ACCESS_EXEC | GDT_ACCESS_RW,
    0xCF); // Kernel code segment
    setGDTGate(2, 0, 0xFFFFFFFF,
    GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_TYPE | GDT_ACCESS_RW,
    0xCF); // Kernel data segment

    setGDTGate(3, 0, 0xFFFFFFFF,
    GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_TYPE | GDT_ACCESS_EXEC | GDT_ACCESS_RW,
    0xCF); // Userland code segment
    setGDTGate(4, 0, 0xFFFFFFFF,
    GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_TYPE | GDT_ACCESS_RW,
    0xCF); // Userland data segment

    writeTSS(5, 0x10, 0);

    gdt_flush((uint32_t)&gdt_ptr);
    // TODO: Implement Interrupt Stack Table
    //tss_flush();
}

void setGDTGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entries[num].base_low   = (base & 0xFFFF);
    gdt_entries[num].base_mid   = (base >> 16) & 0xFF;
    gdt_entries[num].base_high  = (base >> 24) & 0xFF;

    gdt_entries[num].limit = (limit & 0xFFFF);
    gdt_entries[num].flags = (limit >> 16) & 0x0F;
    gdt_entries[num].flags |= (granularity & 0xF0);

    gdt_entries[num].access = access;
}

void writeTSS(uint32_t num, uint16_t ss0, uint32_t esp0) {
    uint32_t base = (uint32_t)&tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    setGDTGate(num, base, limit,
    GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_EXEC | GDT_ACCESS_ACC,
    0x00);
    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;

    tss_entry.cs = 0x08 | 0x3;
    tss_entry.ss = 0x10 | 0x3;
    tss_entry.ds = 0x10 | 0x3;
    tss_entry.es = 0x10 | 0x3;
    tss_entry.fs = 0x10 | 0x3;
    tss_entry.gs = 0x10 | 0x3;
}