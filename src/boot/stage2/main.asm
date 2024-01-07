org 0x7E00
bits 32

main:
    ; Setup segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    ; Reenable NMI
    in al, CMOSRegisterB                            ; Read CMOS register B
    and al, 0x7F                                    ; Unset the high bit to enable NMI
    out CMOSRegisterB, al
    in al, CMOSRegisterC                            ; Dummy read CMOS register C to makesure the update takes place

    sti

    ; Print hello, world!
    mov esi, msg_hello
    mov edi, ScreenBuffer

.loop:
    lodsb
    or al, al
    jz .done

    mov [edi], al
    inc edi
    inc edi
    jmp .loop

.done:

halt:
    jmp halt

CMOSRegisterB                       equ 0x70
CMOSRegisterC                       equ 0x71

ScreenBuffer                        equ 0xB8000

msg_hello:                          db 'Hello world from protected mode!', 0