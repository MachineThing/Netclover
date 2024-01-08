org 0x7E00                                          ; Bootsector start + an extra sector
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
    
    call print
    ;jmp halt

    cli

    jmp word 18h:goreal

print:
    push ax
    .loop:
        lodsb
        or al, al
        jz .done

        mov [edi], al
        inc edi
        inc edi
        jmp .loop

    .done:
        pop ax
        ret

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

msg_hello:                          db 'Hello world from protected mode!', 0
msg_hellor:                          db 'Hello world from real mode!', 0