bits 32

%macro ISR_NOCODE 1
    global isr%1
    isr%1:
        cli
        push long 0
        push long %1
        jmp isr_common_stub
%endmacro

%macro ISR_CODE 1
    global isr%1
    isr%1:
        cli
        push long %1
        jmp isr_common_stub
%endmacro

global idt_flush
idt_flush:
    mov eax, [esp+4]
    lidt [eax]
    sti                 ; Enable interrupts
    ret

ISR_NOCODE  0           ; Division error
ISR_NOCODE  1           ; Debug
ISR_NOCODE  2           ; Non-maskable interrupt
ISR_NOCODE  3           ; Breakpoint
ISR_NOCODE  4           ; INTO Overflow
ISR_NOCODE  5           ; Bound Range Exceeded
ISR_NOCODE  6           ; Invalid Opcode
ISR_NOCODE  7           ; Device Not Available
ISR_CODE    8           ; Double Fault
ISR_NOCODE  9           ; Coprocessor Segment Overrun
ISR_CODE    10          ; Invalid TSS
ISR_CODE    11          ; Segment Not Present
ISR_CODE    12          ; Stack-Segment Fault
ISR_CODE    13          ; General Protection Fault
ISR_CODE    14          ; Page Fault
ISR_NOCODE  15          ; Reserved
ISR_NOCODE  16          ; x86 Floating-Point Exception
ISR_NOCODE  17          ; Alignment Check
ISR_NOCODE  18          ; Machine Check
ISR_NOCODE  19          ; SIMD Floating-Point Exception
ISR_NOCODE  20          ; Virtualization Exception
ISR_NOCODE  21          ; Control Protection Exception
ISR_NOCODE  22          ; Reserved
ISR_NOCODE  23          ; Reserved
ISR_NOCODE  24          ; Reserved
ISR_NOCODE  25          ; Reserved
ISR_NOCODE  26          ; Reserved
ISR_NOCODE  27          ; Reserved
ISR_NOCODE  28          ; Hypervisor Injection Exception
ISR_NOCODE  29          ; VMM Communication Exception
ISR_NOCODE  30          ; Security Exception
ISR_NOCODE  31          ; Reserved
ISR_NOCODE  127
ISR_NOCODE  128

extern isr_handler
isr_common_stub:
    pusha
    mov eax, ds
    push eax
    mov eax, cr2
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call isr_handler

    ADD esp, 8
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8
    sti
    iret