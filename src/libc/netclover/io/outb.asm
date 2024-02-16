bits 32
global outb

outb:
    mov dx, [esp + 4]   ; Port
    mov al, [esp + 8]   ; Value
    out dx, al
    ret