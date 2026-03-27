;bios data area bytes D0-EF are free real estate. use it for stuff. BDA at 0x3000 seems like a good spot
;D0 bit 1-0 = installed video card. 0 = vga. 1 = v9958. 2 = ice 40/tgss, 3 = other
;D0 bits 2-4: reserved. Nothing right now, but expect for this to do something else in the future
;D0 bit 5: 0 if this system has a 8224 PIT, 1 if it has a ice 40 fpga microsecond timer
;D0 bit 6. if system entry is 0x10000, this is 0. if system entry is 0x100000, this is 1
;D0 bit 7. if 1, keyboard port is at 61. if 0, it's a "normal" ps2 keyboard setup
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