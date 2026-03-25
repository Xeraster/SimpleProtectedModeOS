CPU 486
BITS 16
org 0xFF00              ; offset within F000 segment

biosToRam:
    cli

    ; setup stack
    mov ax, 0x3000
    mov ss, ax
    mov sp, 0x7FFF

    ; source = F000:FD00  (maps to top of BIOS)
    mov ax, 0xF000
    mov ds, ax

    ; destination = 0000:8000
    mov ax, 0
    mov es, ax

    mov si, 0xFD00
    mov di, 0x8000
    mov cx, 0x0200      ; 512 bytes

copy_loop:
    mov al, [cs:si]
    mov [es:di], al
    inc si
    inc di
    loop copy_loop
    ;out 0xE9, al
    jmp 0x0000:0x8000

; pad to reset vector
times 0xF0-($-$$) db 0x90

; reset vector at F000:FFF0
nop
nop
jmp biosToRam

times 0x100-($-$$) db 0x90