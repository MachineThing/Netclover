bits 32
; Prekernel / Stage 2.5 bootloader / Multiboot start point

extern main
; Kernel is located at 0x100000 in phys memory

section .kinit
kinit:
    ; Create temp GDT
    xchg bx, bx
    lgdt [init_GDTDesc-0xC0000000]
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

    jmp 0x18:main

section .data
align 32
init_page_pml4:
dq ((init_page_dir_tab-$$-0xC0000000) | 1) + $$     ; 0x0
times 512-1 dq 0

init_page_dir_tab:
; Note: We subtract $$ from the directory pointer
; and add $$ to it later so it can be logical or'ed
; Not doing the above will result in the assembler complaining
dq ((init_page_dir-$$-0xC0000000) | 1) + $$         ; 0x0
dq 0
dq 0
dq ((init_page_dir-$$-0xC0000000) | 1) + $$         ; 0xC0000000
times 512-4 dq 0

align 4096
init_page_dir:
dq 10000011b                                        ; Initial 4mb identity map
times 512-1 dq 0

init_GDT:
            dq 0                                                        ; NULL Descriptor

            ; 32-bit code segment
            dw 0FFFFh                                                   ; Limit (bits 0-15) = 0xFFFF for full 32-bit range
            dw 0                                                        ; Base (bits 0-15) = 0x0
            db 0                                                        ; Base (bits 16-23)
            db 10011010b                                                ; Access (present, ring 0, code segment, executable, direction 0, readable) + access bit
            db 11001111b                                                ; Flags (4k pages, 32-bit mode, no longmode, reserved) + limit (bits 16-19)
            db 0                                                        ; Base high

            ; 32-bit data segment
            dw 0FFFFh                                                   ; Limit (bits 0-15) = 0xFFFF for full 32-bit range
            dw 0                                                        ; Base (bits 0-15) = 0x0
            db 0                                                        ; Base (bits 16-23)
            db 10010010b                                                ; Access (present, ring 0, data segment, executable, direction 0, writeable) + access bit
            db 11001111b                                                ; Flags (4k pages, 32-bit mode, no longmode, reserved) + limit (bits 16-19)
            db 0                                                        ; Base high

            ; 64-bit code segment
            dw 0FFFFh                                                   ; Limit (bits 0-15) = 0xFFFF for full 32-bit range
            dw 0                                                        ; Base (bits 0-15) = 0x0
            db 0                                                        ; Base (bits 16-23)
            db 10011010b                                                ; Access (present, ring 0, code segment, executable, direction 0, readable) + access bit
            db 10101111b                                                ; Flags (4k pages, 32-bit mode, no longmode, reserved) + limit (bits 16-19)
            db 0                                                        ; Base high

            ; 64-bit data segment
            dw 0FFFFh                                                   ; Limit (bits 0-15) = 0xFFFF for full 32-bit range
            dw 0                                                        ; Base (bits 0-15) = 0x0
            db 0                                                        ; Base (bits 16-23)
            db 10010010b                                                ; Access (present, ring 0, data segment, executable, direction 0, writeable) + access bit
            db 10101111b                                                ; Flags (4k pages, 32-bit mode, no longmode, reserved) + limit (bits 16-19)
            db 0                                                        ; Base high

init_GDTDesc:
            dw (init_GDTDesc-0xC0000000) - (init_GDT-0xC0000000) - 1    ; Limit = size of GDT
            dd (init_GDT-0xC0000000)                                    ; Address of GDT