bits 32

extern main
; Kernel is located at 0x100000 in phys memory

section .kinit
kinit:
    ; Setup basic paging
    mov eax, (init_page_pml4 - 0xC0000000)   ; Location in phys memory
    mov cr3, eax

    ; Enable PAE
    mov edx, cr4
    or edx, (1 << 5)
    mov cr4, edx

    ; Set LM Bit
    mov ecx, 0xC0000080          ; 0xC0000080 is the EFER MSR.
    rdmsr
    or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
    wrmsr

    ; Enable paging
    mov ebx, cr0
    or ebx, (1 << 31)
    mov cr0, ebx

    jmp main

section .data
align 32
init_page_pml4:
dq ((init_page_dir_tab-$$-0xC0000000) | 1) + $$     ; 0x0
times 512-1 dq 0

init_page_dir_tab:
; Note: We subtract $$ from the directory pointer
; and add $$ to it later so it can be logical or'ed
; Not doing the above will result in the assembler complaining
dq ((init_page_dir-$$-0xC0000000) | 1) + $$     ; 0x0
dq 0
dq 0
dq ((init_page_dir-$$-0xC0000000) | 1) + $$     ; 0xC0000000
times 512-4 dq 0

align 4096
init_page_dir:
dq 10000011b                    ; Initial 4mb identity map
times 512-1 dq 0