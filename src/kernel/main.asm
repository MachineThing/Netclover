bits 32

main:
    ; Print hello world message
    mov esi, msg_hello
    mov edi, ScreenBuffer
    call print
    jmp halt

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

ScreenBuffer                        equ 0xB8000

msg_hello:                          db 'Hello, world from kernel!', 0