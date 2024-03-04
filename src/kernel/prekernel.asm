bits 32

extern main
; Kernel is located at 0x100000 in phys memory

section .kinit
kinit:
    xchg bx, bx

    ; Setup basic paging
    mov eax, (init_page_dir - 0xC0000000)
    mov cr3, eax

    ; Enable PAE
    mov edx, cr4
    or edx, (1 << 4)
    mov cr4, edx

    ; Enable paging
    mov ebx, cr0
    or ebx, (1 << 31)
    mov cr0, ebx

    jmp main

section .data
align 4096
init_page_dir:
    dd 10000011b                ; Initial 4mb identity map
    times 768-1 dd 0            ; Padding

    dd (0 << 22) | 10000011b    ; 0xC0000000
    dd (1 << 22) | 10000011b
    dd (2 << 22) | 10000011b
    dd (3 << 22) | 10000011b
    times 256-4 dd 0            ; Padding