bits __BITS__
global inb

inb:
    %if __BITS__ = 64
        ; In long mode, arguments are in registers
        mov dx, di          ; Port
    %else
        mov dx, [esp + 4]   ; Port
    %endif
    xor eax, eax        ; Incoming value is stored in eax
    in al, dx
    ret