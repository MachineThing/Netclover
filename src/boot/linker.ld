OUTPUT_FORMAT("binary")

SECTIONS {
    . = 0x7c00;
    .text :
    {
        boot.o(.text)
    }
    .data :
    {
        boot.o(.data)
    }
    . = 0x7e00;
    .text :
    {
        *(.text)
    }
    .data :
    {
        *(.data)
    }
    .rodata :
    {
        *(.rodata)
    }
    .bss :
    {
        *(.bss)
    }

    reserved_sectors = (. - 0x7c00) / 512 + 1;
}