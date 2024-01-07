org 0x7C00                                          ; Bootsector start
bits 16

; FAT 12 Header
jmp short init
nop

bdb_oem:                    db 'MSWIN4.1'           ; 8 bytes
bdb_bytes_per_sector:       dw 512
bdb_sectors_per_cluster:    db 1
bdb_reserved_sectors:       dw 4
bdb_fat_count:              db 2
bdb_dir_entries_count:      dw 0E0h
bdb_total_sectors:          dw 2880                 ; 2880 * 512 = 1.44MB
bdb_media_descriptor_type:  db 0F0h                 ; F0 = 3.5" floppy disk
bdb_sectors_per_fat:        dw 9                    ; 9 sectors/fat
bdb_sectors_per_track:      dw 18
bdb_heads:                  dw 2
bdb_hidden_sectors:         dd 0
bdb_large_sector_count:     dd 0

; extended boot record
ebr_drive_number:           db 0                    ; 0x00 floppy, 0x80 hdd, useless
                            db 0                    ; reserved
ebr_signature:              db 29h
ebr_volume_id:              db 03h, 17h, 20h, 04h   ; serial number, value doesn't matter
ebr_volume_label:           db 'HTTP OS    '        ; 11 bytes, padded with spaces
ebr_system_id:              db 'FAT12   '           ; 8 bytes

; Code
init:
    ; Setup data segments
    xor ax, ax                                      ; Set AX to 0
    mov ds, ax
    mov es, ax

    ; Setup stack
    mov ss, ax
    mov sp, 0x7C00

    ; BIOSes may incorrectly start us at 07C0:0000 instead of 0000:7C00
    push es
    push word main
    retf

main:
    ; BIOS should set DL to drive number
    mov [ebr_drive_number], dl

    ; Read stage 2 bootloader
    mov ax, 1
    mov cl, 3
    mov bx, 0x7E00
    call disk_read

    ; Switch to protected mode
    cli                                             ; Disable interrupts
    ; Disable NMI interrupts (as suggested by the Intel Developers Manual)
    in al, CMOSRegisterB                            ; Read CMOS register B
    or al, 0x80                                     ; Set the high bit to disable NMI
    out CMOSRegisterB, al
    in al, CMOSRegisterC                            ; Dummy read CMOS register C to makesure the update takes place
    
    call EnableA20                                  ; Enable A20
    lgdt [g_GDTDesc]                                ; Load Global Descriptor Table

    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp dword 08h:0x7E00
;;;;;;;;;;;;;;;;;;;;;;;;;;
; Protected mode Helpers ;
;;;;;;;;;;;;;;;;;;;;;;;;;;

; Enables the A20 gate
EnableA20:
    ; Disable keyboard
    call A20WaitInput
    mov al, KbdControllerDisableKeyboard
    out KbdControllerCommandPort, al

    ; Read control output port
    call A20WaitInput
    mov al, KbdControllerReadCtrlOutputPort
    out KbdControllerCommandPort, al

    call A20WaitOutput
    in al, KbdControllerDataPort
    push ax

    ; Write control output port
    call A20WaitInput
    mov al, KbdControllerWriteCtrlOutputPort
    out KbdControllerCommandPort, al

    call A20WaitInput
    pop ax
    or al, 2                                        ; Bit 2 = A20 Gate bit
    out KbdControllerDataPort, al

    ; Reenable keyboard
    call A20WaitInput
    mov al, KbdControllerEnableKeyboard
    out KbdControllerCommandPort, al

    call A20WaitInput
    ret

A20WaitInput:
    ; wait until status bit 2 (input buffer) is 0
    ; by reading from command port, we read status byte
    in al, KbdControllerCommandPort
    test al, 2
    jnz A20WaitInput
    ret

A20WaitOutput:
    ; Wait until status bit 1 (output buffer) is 1 so it can be read
    in al, KbdControllerCommandPort
    test al, 1
    jz A20WaitOutput
    ret

;;;;;;;;;;;;;
; Functions ;
;;;;;;;;;;;;;

; Prints a error to the screen, and restarts on key press
; Parameters:
;   - SI: Error message pointer
error:
    call print
    mov si, press_key
    call print

    ; Wait for key press
    mov ah, 00h
    int 16h                                         ; BIOS interrupt
    jmp 0FFFFh:0h                                   ; Restart the computer

; Prints a message to the screen
; Parameters:
;   - SI: Message pointer
print:
    push si
    push ax
    push bx

    .loop:
        lodsb
        or al, al
        jz .done

        mov ah, 0x0E                                ; Print character
        mov bh, 0                                   ; Set page number to 0
        int 0x10                                    ; Call bios interrupt

        jmp .loop
    
    .done:
        pop bx
        pop ax
        pop si
        ret

; Disk routines

; Converts a LBA address to a CHS address
; Parameters:
;   - AX: LBA address
; Returns:
;   - CX (bits 0-5): Sector number
;     CX (bits 6-15): Cylinder
;   - DH: Head
lba_to_chs:
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

; Reads sectors from a disk
; Parameters:
;   - AX: LBA address
;   - CL: Number of sectors to read (up to 128)
;   - DL: Drive number
;   - ES:BX: Memory address where to store read data
disk_read:
    pusha                                           ; Save all registers (saves code by not repeating push instruction)
    push cx                                         ; Temporarily save CL (Number of sectors to read)
    call lba_to_chs                                 ; Compute CHS
    pop ax                                          ; AL = Number of Sectors to read

    mov ah, 02h
    mov di, 3                                       ; Retry count

    .retry:
        pusha                                       ; Save all registers
        stc                                         ; Set carry flag (as some BIOS'es don't set it)
        int 13h                                     ; Carry flag cleared = success
        popa
        jnc .done

        ; Read failed
        call disk_reset
        dec di
        test di, di
        jnz .retry

    .failure:
        mov si, disk_read_fail
        jmp error

    .done:
        popa
        ret

; Resets the disk controller
; Parameters:
;   - DL: Drive number
disk_reset:
    pusha
    mov ah, 0
    stc
    int 13h
    jc .failure
    popa
    ret

    .failure:
        mov si, disk_reset_fail
        jmp error

; Messages
%define ENDL 0x0D, 0x0A, 0x00
disk_read_fail:                     db 'Failed to read from disk', ENDL
disk_reset_fail:                    db 'Failed to reset disk', ENDL
press_key:                          db 'Press any key to reboot', ENDL

; Magic numbers
CMOSRegisterB                       equ 0x70
CMOSRegisterC                       equ 0x71
KbdControllerDataPort               equ 0x60
KbdControllerCommandPort            equ 0x64
KbdControllerDisableKeyboard        equ 0xAD
KbdControllerEnableKeyboard         equ 0xAE
KbdControllerReadCtrlOutputPort     equ 0xD0
KbdControllerWriteCtrlOutputPort    equ 0xD1

A20TestMagic                        equ 0x1967

g_GDT:
            dq 0                                    ; NULL Descriptor

            ; 32-bit code segment
            dw 0FFFFh                               ; Limit (bits 0-15) = 0xFFFF for full 32-bit range
            dw 0                                    ; Base (bits 0-15) = 0x0
            db 0                                    ; Base (bits 16-23)
            db 10011010b                            ; Access (present, ring 0, code segment, executable, direction 0, readable) + access bit
            db 11001111b                            ; Granularity (4k pages, 32-bit mode, no longmode, reserved) + limit (bits 16-19)
            db 0                                    ; Base high

            ; 32-bit data segment
            dw 0FFFFh                               ; Limit (bits 0-15) = 0xFFFF for full 32-bit range
            dw 0                                    ; Base (bits 0-15) = 0x0
            db 0                                    ; Base (bits 16-23)
            db 10010010b                            ; Access (present, ring 0, data segment, executable, direction 0, writeable) + access bit
            db 11001111b                            ; Granularity (4k pages, 32-bit mode, no longmode, reserved) + limit (bits 16-19)
            db 0                                    ; Base high

            ; 16-bit code segment
            dw 0FFFFh                               ; Limit (bits 0-15) = 0xFFFF
            dw 0                                    ; Base (bits 0-15) = 0x0
            db 0                                    ; Base (bits 16-23)
            db 10011010b                            ; Access (present, ring 0, code segment, executable, direction 0, readable) + access bit
            db 00001111b                            ; Granularity (1b pages, 16-bit mode, no longmode, reserved) + limit (bits 16-19)
            db 0                                    ; Base high

            ; 16-bit data segment
            dw 0FFFFh                               ; Limit (bits 0-15) = 0xFFFF
            dw 0                                    ; Base (bits 0-15) = 0x0
            db 0                                    ; Base (bits 16-23)
            db 10010010b                            ; Access (present, ring 0, data segment, executable, direction 0, readable) + access bit
            db 00001111b                            ; Granularity (1b pages, 16-bit mode, no longmode, reserved) + limit (bits 16-19)
            db 0                                    ; Base high

g_GDTDesc:
            dw g_GDTDesc - g_GDT - 1                ; Limit = size of GDT
            dd g_GDT                                ; Address of GDT

times 510-($-$$) db 0
                dw 0xAA55   ; Boot signature

buffer: