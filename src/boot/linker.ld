OUTPUT_FORMAT("binary")

SECTIONS {
    . = 0x7c00;
    .text_boot :
    {
        boot.o(.text)
    }
    .data_boot :
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