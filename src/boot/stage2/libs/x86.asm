%macro enterRealMode 0
bits 32
    push ax
    jmp word 18h:.pmode16

bits 16
.pmode16:
    mov eax, cr0
    and al, ~1
    mov cr0, eax
    jmp word 00h:.rmode
.rmode:
    xor ax, ax
    mov ds, ax
    mov ss, ax
    pop ax
    sti
%endmacro

%macro enterProtectedMode 0
bits 16
    push ax
    cli
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp dword 08h:.endingPM

bits 32
.endingPM:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    pop ax
%endmacro

%macro newCallFrame 0
    push ebp
    mov ebp, esp
%endmacro

%macro restoreCallFrame 0
    mov esp, ebp
    pop ebp
%endmacro

; Convert linear address to segment:offset address
; Args:
;    1 - linear address
;    2 - (out) target segment (e.g. es)
;    3 - target 32-bit register to use (e.g. eax)
;    4 - target lower 16-bit half of #3 (e.g. ax)

%macro LinearToSegOffset 4
    mov %3, %1      ; linear address to eax
    shr %3, 4
    mov %2, %4
    mov %3, %1      ; linear address to eax
    and %3, 0xf
%endmacro

bits 32

global outb
global inb
global diskReset16
global diskRead16
global diskGetDriveParams16
global getVendorId

extern boot_disk_reset

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

;uint16_t __attribute__((cdecl)) diskReset16(int8_t drive);
diskReset16:
    bits 32
    push dx
    mov dl, [esp + 4]
    enterRealMode
    bits 16
    call boot_disk_reset
    enterProtectedMode
    bits 32
    pop dx
    ret

;uint16_t __attribute__((cdecl)) diskRead16(int8_t drive, uint16_t cylinder, uint16_t head, uint16_t sector, uint8_t count, uint8_t* dataOut);
diskRead16:
    bits 32
    
    newCallFrame
    enterRealMode
    bits 16
    push ebx
    push es

    mov dl, [bp + 8]                               ; DL = Drive number

    mov ch, [bp + 12]                               ; CH = Cylinder to bits 8-15

    mov cl, [bp + 13]                               ; CL = Cylinder to bits 6-7
    shl cl, 6

    mov dh, [bp + 16]                               ; DH = Head

    mov al, [bp + 20]                              ; CL = Sector to bits 0-5
    and al, 3Fh
    or cl, al

    mov al, [bp + 24]                              ; AL = count

    LinearToSegOffset [bp + 28], es, ebx, bx
    
    mov ah, 02h
    int 13h

    ; Status is stored in AH
    mov al, ah
    xor ah, ah

.ending:
    pop es
    pop ebx
    enterProtectedMode
    bits 32
    restoreCallFrame
    ret

;uint16_t __attribute__((cdecl)) diskGetDriveParams16(int8_t drive, uint8_t* driveTypeOut, uint16_t* cylindersOut, uint16_t* headsOut, uint16_t* sectorsOut);
diskGetDriveParams16:
    bits 32

    newCallFrame
    enterRealMode
    bits 16

    push es
    push bx
    push esi
    push di

    mov dl, [bp + 8]                               ; DL = Drive number

    mov ah, 08h

    ; Guard against BIOS bugs...
    xor di, di                                      ; DI = 0
    mov es, di
    stc                                             ; Set carry flag
    int 13h                                         ; Get drive parameters

    ; Return
    jnc .flag_clear
    mov ax, 1                                       ; AX = 1 on error
    jmp .ending
.flag_clear:
    xor ax, ax                                      ; AX = 0 on success
.set_params:
    ; Drive type
    LinearToSegOffset [bp + 12], es, esi, si
    mov [es:si], bl                                 ; BL = Drive type

    ; Cylinders
    mov bl, ch                                      ; CH = Low eight bits of max cylinder number
    mov bh, cl                                      ; CL (bits 0-5) = Max sector number
    shr bh, 6
    inc bx

    LinearToSegOffset [bp + 16], es, esi, si
    mov [es:si], bx

    ; Sectors
    xor ch, ch                                      ; CL (bits 6-7) = Sector number
    and cl, 3Fh
    LinearToSegOffset [bp + 24], es, esi, si
    mov [es:si], cx

    ; Heads
    mov cl, dh                                      ; DH = Heads
    inc cx
    LinearToSegOffset [bp + 20], es, esi, si
    mov [es:si], cx

.ending:
    pop di
    pop esi
    pop bx
    pop es
    enterProtectedMode
    bits 32
    restoreCallFrame
    ret

getVendorId:
    bits 32
    newCallFrame

    ; Get vendor ID and write to pointer
    push eax
    push ecx
    push ebx
    push eax

    xor eax, eax
    cpuid
    mov eax, [ebp+8]
    mov dword [eax], ebx
    mov dword [eax+4], edx
    mov dword [eax+8], ecx
    xor bl, bl
    mov byte  [eax+12], bl

    pop eax
    pop ebx
    pop ecx
    pop edx
    restoreCallFrame
    ret