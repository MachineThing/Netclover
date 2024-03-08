bits 64

%macro ISR_NOCODE 1
    global isr%1
    isr%1:
        cli
        push 0
        push %1
        jmp isr_common_stub
%endmacro

%macro ISR_CODE 1
    global isr%1
    isr%1:
        cli
        ; Error code is already on stack
        push %1
        jmp isr_common_stub
%endmacro

%macro IRQ 2
    global isq%1
    isq%1:
        cli
        push 0
        push %2
        jmp irq_common_stub
%endmacro

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

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
IRQ 16, 48

ISR_NOCODE  127
ISR_NOCODE  128

extern isr_handler
extern irq_handler

%macro pushall 0
    push rax
    push rbx
    push rcx
    push rdx
%endmacro

%macro popall 0
    pop rdx
    pop rcx
    pop rbx
    pop rax
    add rsp, 16         ; Destroys interrupt number and error code
%endmacro popall

isr_common_stub:
    pushall

    mov rdi, rsp
    cld
    call isr_handler
    mov rsp, rax

    popall
    iretq

irq_common_stub:
    pushall

    mov rdi, rsp
    cld
    call irq_handler

    popall
    iretq