org 0x7C00
bits 16

; FAT 12 Header
jmp short main
nop

bdb_oem:                    db 'MSWIN4.1'           ; 8 bytes
bdb_bytes_per_sector:       dw 512
bdb_sectors_per_cluster:    db 1
bdb_reserved_sectors:       dw 1
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
main:
    ; Setup data segments
    xor ax, ax                                      ; Set AX to 0
    mov ds, ax
    mov es, ax

    ; Setup stack
    mov ss, ax
    mov sp, 0x7C00

    ; Read something from the floppy
    mov [ebr_drive_number], dl

    mov ax, 1
    mov cl, 1
    mov bx, 0x7E00

    call disk_read

halt:
    cli
    hlt

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
        mov si, read_error
        call print
        jmp halt

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
        mov si, read_error
        call print
        jmp halt

; Messages
%define ENDL 0x0D, 0x0A, 0x00
read_error:  db 'Read failure', ENDL
disk_error:  db 'Disk reset failure', ENDL

times 510-($-$$) db 0
    dw 0xAA55  ; Boot signature