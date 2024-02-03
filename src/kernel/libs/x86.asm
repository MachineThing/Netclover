%macro newCallFrame 0
    push ebp
    mov ebp, esp
%endmacro

%macro restoreCallFrame 0
    mov esp, ebp
    pop ebp
%endmacro

bits 32

global outb
global inb

outb:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

inb:
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx
    ret