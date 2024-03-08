bits 64
global gdt_flush
global tss_flush

gdt_flush:
    lgdt [rdi]      ; gdt_ptr is passed in rdi register

    ; Set segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    pop rax
    push 0x8
    push rax

    retfq          ; Far return (lretq in gas syntax)

tss_flush:
    mov ax, 0x2b
    ltr ax          ; Load task register
    ret