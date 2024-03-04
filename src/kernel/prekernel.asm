bits 32

extern main
; Kernel is located at 0x100000 in phys memory

section .kinit
kinit:
    ; Setup basic paging
    mov eax, (init_page_dir_tab - 0xC0000000)   ; Location in phys memory
    mov cr3, eax

    ; Enable PAE
    mov edx, cr4
    or edx, (1 << 5)
    mov cr4, edx

    ; Enable paging
    mov ebx, cr0
    or ebx, (1 << 31)
    mov cr0, ebx
    xchg bx, bx

    jmp main

section .data
align 32
init_page_dir_tab:
; Note: We subtract $$ from the directory pointer
; and add $$ to it later so it can be logical or'ed
; Not doing the above will result in the assembler complaining
dq ((init_page_dir-$$-0xC0000000) | 1) + $$     ; 0x0
dq 0
dq 0
dq ((init_page_dir-$$-0xC0000000) | 1) + $$     ; 0xC0000000

align 4096
init_page_dir:
dq 10000011b                    ; Initial 4mb identity map
times 512-1 dq 0