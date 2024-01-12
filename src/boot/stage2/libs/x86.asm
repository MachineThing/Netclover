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

bits 32

global outb
global inb
global diskReset16
global diskRead16
global diskGetDriveParams16
global biosPrintTest

extern bdb_sectors_per_track
extern bdb_heads
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

;void __attribute__((cdecl)) diskReset16(int8_t drive);
diskReset16:
    bits 32
    mov dl, [esp + 4]
    enterRealMode
    bits 16
    mov ah, 0
    stc
    int 13h
    enterProtectedMode
    bits 32
    ret

;void __attribute__((cdecl)) diskRead16(int8_t drive, uint16_t cylinder, uint16_t head, uint16_t sector, uint8_t count, uint8_t far* dataOut);
diskRead16:
    nop

;void __attribute__((cdecl)) diskGetDriveParams16(int8_t drive, uint8_t* driveTypeOut, uint16_t* cylindersOut, uint16_t* sectorsOut, uint16_t* headsOut);
diskGetDriveParams16:
    nop

biosPrintTest:
    bits 32
    mov si, [esp + 4]
    enterRealMode
    bits 16
    .loop:
        lodsb
        or al, al
        jz .done

        mov ah, 0x0E                                ; Print character
        mov bh, 0                                   ; Set page number to 0
        int 0x10                                    ; Call bios interrupt

        jmp .loop
    
    .done:
        enterProtectedMode
        bits 32
        ret

; Converts a LBA address to a CHS address
; Parameters:
;   - AX: LBA address
; Returns:
;   - CX (bits 0-5): Sector number
;     CX (bits 6-15): Cylinder
;   - DH: Head
lba_to_chs:
    bits 16
    push ax
    push dx

    xor dx, dx                                      ; DX = 0
    div word [bdb_sectors_per_track]                ; AX = LBA / SectorsPerTrack
                                                    ; DX = LBA % SectorsPerTrack
    
    inc dx                                          ; DX = (LBA % SectorsPerTrack + 1) = Sector
    mov cx, dx                                      ; CX = Sector

    xor dx, dx                                      ; DX = 0
    div word [bdb_heads]                            ; AX = (LBA / SectorsPerTrack) / Heads = Cylinder
                                                    ; DX = (LBA / SectorsPerTrack) % Heads = Head

    mov dh, dl                                      ; DH = head
    mov ch, al                                      ; CH = cylinder (lower 8 bits)
    shl ah, 6
    or cl, ah                                       ; Put upper 2 bits of cylinder in CL

    pop ax
    mov dl, al                                      ; Restore DL
    pop ax
    ret