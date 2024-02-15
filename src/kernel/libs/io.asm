bits 32

global outb
global inb

outb:
    mov dx, [esp + 4]   ; Port
    mov al, [esp + 8]   ; Value
    out dx, al
    ret

inb:
    mov dx, [esp + 4]   ; Port
    xor eax, eax        ; Incoming value is stored in eax
    in al, dx
    ret