bits 64
global gdt_flush
global tss_flush

gdt_flush:
    lgdt [rdi]      ; gdt_ptr is passed in rdi register

    ; Set segments
    mov rax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp .flush
.flush:
    dw 0x08
    ret

tss_flush:
    mov ax, 0x2b
    ltr ax          ; Load task register
    ret