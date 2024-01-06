org 0x7C00                                          ; Bootsector start
bits 16

; FAT 12 Header
jmp short init
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

KERNEL_LOAD_SEGMENT:        equ 0x2000
KERNEL_LOAD_OFFSET:         equ 0

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

    ; Get drive parameters instead of relying on disk data
    push es
    mov ah, 08h
    int 13h
    xor al, al                          ; AL = 0
    jc error
    pop es

    and cl, 0x3F                        ; remove top 2 bits
    xor ch, ch
    mov [bdb_sectors_per_track], cx     ; sector count

    inc dh
    mov [bdb_heads], dh                 ; head count

    ; Read FAT root directory
    ; Compute LBA of root directory = reserved_sectors + fats * sectors_per_fat
    mov ax, [bdb_sectors_per_fat]                   ; AX = sectors_per_fat
    mov bl, [bdb_fat_count]
    xor bh, bh
    mul bx                                          ; AX *= fats
    add ax, [bdb_reserved_sectors]                  ; AX = LBA of root directory
    push ax

    ; Compute size of root directory = (32 * number_of_entries) / bytes_per_sector
    mov ax, [bdb_dir_entries_count]
    shl ax, 5                                       ; AX *= 32
    xor dx, dx                                      ; dx = 0
    div word [bdb_bytes_per_sector]                 ; number of sectors we need to read

    test dx, dx                                     ; if division remainder != 0, then add 1
    jz root_dir_after                               ; this means we haev a sector only partially filled with entries
    inc ax

root_dir_after:
    ; Read root directory
    mov cl, al                                      ; number of sectors to read = size of root directory
    pop ax                                          ; LBA of root directory
    mov dl, [ebr_drive_number]                      ; DL = drive number (We saved it on line 49)
    mov bx, buffer                                  ; es:bx = buffer
    call disk_read

    ; Search for kernel.bin
    xor bx, bx
    mov di, buffer

search_kernel:
    mov si, kernel_file
    mov cx, 11                                      ; Compare up to 11 characters (8 for filename and 3 for extension)
    push di
    repe cmpsb
    pop di
    je found_kernel

    ; We didn't find kernel yet
    add di, 32
    inc bx
    cmp bx, [bdb_dir_entries_count]
    jl search_kernel

    ; We can't find the kernel, time to panic
    mov al, 1
    jmp error

found_kernel:
    ; DI should have address to entry
    mov ax, [di+26]                                 ; First logical cluster field
    mov [kernel_cluster], ax

    ; Load FAT from disk into memory
    mov ax, [bdb_reserved_sectors]
    mov bx, buffer
    mov cl, [bdb_sectors_per_fat]
    mov dl, [ebr_drive_number]
    call disk_read

    ; Read kernel and process FAT chain
    mov bx, KERNEL_LOAD_SEGMENT
    mov es, bx
    mov bx, KERNEL_LOAD_OFFSET

load_kernel:
    mov ax, [kernel_cluster]

    add ax, 31                                      ; TODO: Make this not hardcoded
    
    mov cl, 1
    mov dl, [ebr_drive_number]
    call disk_read

    add bx, [bdb_bytes_per_sector]

    ; Compute location of next cluster
    mov ax, [kernel_cluster]
    mov cx, 3
    mul cx
    mov cx, 2
    div cx                                          ; AX = index of entry in FAT, DX = cluster % 2

    mov si, buffer
    add si, ax
    mov ax, [ds:si]                                 ; Read entry from FAT table at index AX

    or dx, dx
    jz load_even

load_odd:
    shr ax, 4
    jmp next_cluster

load_even:
    and ax, 0x0FFF

next_cluster:
    cmp ax, 0x0FF8                                  ; End of chain
    jae read_finish

    mov [kernel_cluster], ax
    jmp load_kernel

read_finish:
    ; Jump to the kernel!
    mov dl, [ebr_drive_number]                      ; Boot device in dl

    mov ax, KERNEL_LOAD_SEGMENT
    mov ds, ax
    mov es, ax

    jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

;;;;;;;;;;;;;
; Functions ;
;;;;;;;;;;;;;

; Prints a error to the screen, and restarts on key press
; Parameters:
;   - AL: Error number
error:
    ; Print messages
    mov si, error_msg
    call print
    add al, 30h                                     ; ASCII offset
    mov ah, 0Eh                                     ; Teletype output
    mov bh, 0
    int 0x10
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
        mov al, 2
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
        mov al, 3
        jmp error

; Messages
%define ENDL 0x0D, 0x0A, 0x00
error_msg:      db 'Error: ', 0x00
press_key:      db 0x0D, 0x0A, 'Press any key to reboot', ENDL

kernel_cluster: dw 0
kernel_file:    db 'KERNEL  BIN'

times 510-($-$$) db 0
                dw 0xAA55   ; Boot signature

buffer: