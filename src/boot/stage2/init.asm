bits 32

section .text
    global _start
    extern cmain
    extern ebr_drive_number

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
    
    ; Repush DX as a 32 bit register (EAX)
    mov dl, [ebr_drive_number]
    push dx
    call cmain

    ; Print hello, world!
    ;mov esi, msg_hello
    ;mov edi, ScreenBuffer
    
    ;call print
    jmp halt

halt:
    cli
    hlt


CMOSRegisterB                       equ 0x70
CMOSRegisterC                       equ 0x71

ScreenBuffer                        equ 0xB8000