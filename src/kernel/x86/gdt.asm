bits 32
global gdt_flush
global tss_flush

gdt_flush:
    mov eax, [esp+4]
    lgdt [eax]

    ; Set segments
    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush
.flush:
    ret

tss_flush:
    mov ax, 0x2b
    ltr ax          ; Load task register
    ret