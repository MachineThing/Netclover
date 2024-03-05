bits __BITS__
global outb

outb:
    %if __BITS__ = 64
        ; In long mode, arguments are in registers
        mov dx, di          ; Port
        mov ax, si          ; Value
    %else
        mov dx, [esp + 4]   ; Port
        mov al, [esp + 8]   ; Value
    %endif
    out dx, al
    ret