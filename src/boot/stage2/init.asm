bits 32

section .text
    global _start
    extern cmain

_start:
    ; Setup segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    ; Reenable NMI
    in al, CMOSRegisterB                            ; Read CMOS register B
    and al, 0x7F                                    ; Unset the high bit to enable NMI
    out CMOSRegisterB, al
    in al, CMOSRegisterC                            ; Dummy read CMOS register C to makesure the update takes place

    ;sti

    call cmain

    ; Print hello, world!
    ;mov esi, msg_hello
    ;mov edi, ScreenBuffer
    
    ;call print
    jmp halt

halt:
    cli
    hlt

bits 16
goreal:
    mov eax, cr0
    and al, ~1
    mov cr0, eax

    jmp word 00h:goprotected

goprotected:
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp dword 08h:protagain

bits 32
protagain:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    jmp $


CMOSRegisterB                       equ 0x70
CMOSRegisterC                       equ 0x71

ScreenBuffer                        equ 0xB8000