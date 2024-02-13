#include "gdt.h"

extern void gdt_flush(addr_t);

#define total_entries 5

struct gdt_entry_struct gdt_entries[total_entries];
struct gdt_ptr_struct   gdt_ptr;


void initGDT() {
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * total_entries) - 1;
    gdt_ptr.base = &gdt_entries;

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

    gdt_flush(&gdt_ptr);
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