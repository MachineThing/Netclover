org 0x7C00
bits 16

main:
    ; Setup data segments
    mov ax, 0
    mov ds, ax
    mov es, ax

    ; Setup stack
    mov ss, ax
    mov sp, 0x7C00

    ; Print hello world message
    mov si, msg_hello
    call print

    hlt
    .halt:
        jmp .halt

print:
    push si
    push ax
    push bx

    .loop:
        lodsb
        or al, al
        jz .done

        mov ah, 0x0E    ; Print character
        mov bh, 0       ; Set page number to 0
        int 0x10        ; Call bios interrupt

        jmp .loop
    
    .done:
        pop bx
        pop ax
        pop si
        ret

msg_hello: db 'Hello, world!', 0x0D, 0x0A, 0x00

times 510-($-$$) db 0
    dw 0xAA55  ; Boot signature