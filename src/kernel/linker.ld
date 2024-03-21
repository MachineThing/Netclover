OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x100000;
    .kinit :
    {
        prekernelasm.o(.kinit)
    }
    . += 0xFFFFFFFFC0000000;
    .text ALIGN(4K): AT(ADDR(.text) - 0xFFFFFFFFC0000000)
    {
        *(.text)
    }
    .rodata ALIGN(4K): AT(ADDR(.rodata) - 0xFFFFFFFFC0000000)
    {
        *(.rodata)
    }
    .data ALIGN(4K): AT(ADDR(.data) - 0xFFFFFFFFC0000000)
    {
        *(.data)
    }
    .bss ALIGN(4K): AT(ADDR(.bss) - 0xFFFFFFFFC0000000)
    {
        *(COMMON)
        *(.bss)
    }
}