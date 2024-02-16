bits 32
global inb

inb:
    mov dx, [esp + 4]   ; Port
    xor eax, eax        ; Incoming value is stored in eax
    in al, dx
    ret