;bios data area bytes D0-EF are free real estate. use it for stuff. BDA at 0x3000 seems like a good spot
;D0 bit 1-0 = installed video card. 0 = vga. 1 = v9958. 2 = ice 40/tgss, 3 = other
;D0 bits 2-4: reserved. Nothing right now, but expect for this to do something else in the future
;D0 bit 5: 0 if this system has a 8224 PIT, 1 if it has a ice 40 fpga microsecond timer
;D0 bit 6. if system entry is 0x10000, this is 0. if system entry is 0x100000, this is 1
;D0 bit 7. if 1, keyboard port is at 61. if 0, it's a "normal" ps2 keyboard setup
CPU 486
bits 16
org 0x8000
;this stupid complicated bootloader needs a second stage
_start:
mov bx, 0x3000
mov ss, bx
mov ebp, 07FFCh
mov esp, ebp
cli

;chatgpt says to do this
mov ax, cs
mov ds, ax

jmp bypasslcd
call waitForLcdReady
    mov al, 00111000b
    mov dx, 0A000h
    out dx, al

    ;turn display on
    call waitForLcdReady
    mov al, 00001110b
    mov dx, 0A000h
    out dx, al

    ;set entry mode to the datasheet example default one
    call waitForLcdReady
    mov al, 00000110b
    mov dx, 0A000h
    out dx, al

    ;get the lcd ram counter to reset to zero
    call waitForLcdReady
    mov al, 10000000b          ;reset display data address in address counter to 0
    mov al, 00000010b           ;return home command
    mov dx, 0A000h
    out dx, al

	mov bx, hello
    call printString
bypasslcd:

lgdt [toc] ;load gdt settings

;set cpu in protected mode before jumping to the protected mode c code
mov eax, cr0
;or eax, 00000000000000000000000000000001b
or eax, 0x01
mov cr0, eax

;jmp $+2 			;putting this here seems to at least make it get far enough to execute a hlt instruction once it gets to protected mode
;out 0xE9, al
jmp 08h:pmodecont

;put whatever's in al to the 20x2 lcd screen
charToLcd:
    push ax
        nop
        nop
        nop
        nop
        call waitForLcdReady
        nop
        nop
        nop
        nop
    pop ax
    nop
    nop
    nop
    nop
    mov dx, 0A002h
    out dx, al
ret

;modified print string subroutine from real mode os
;bx = location in *ds* segment of string (wow so much for the possibility of that actually working)
;print location is in cursor values cursorX and cursorY in ram
printString:

        mov al, [cs:bx]
        cmp al, '$'
        je printStringGTFO
        call charToLcd
        inc bx
        jmp printString

        printStringGTFO:

ret
;pre-conditions: none
;post-conditions: the lcd is 100% for sure ready for a command
;registers changed: al
;technology required: working ram
waitForLcdReady:
    mov dx, 0A001h
    in al, dx               ;load the address of whichever port carries the ready signal and save to register al
    and al, 10000000b       ;the busy flag at byte 7 is the only thing I care about right now
    cmp al, 10000000b       
    je waitForLcdReady      ;if bit 7 is 1, the device is not ready. Loop until bit 7 is not 1.
    ;mov cx, 0x0500
    ;call longDelay
ret
hello db 'Hi!$',0

gdt:

; offset 0x0 (0 bytes)
null_descriptor:
	dq 0x0

; offset 0x8 (8 bytes)
code_descriptor:	; cs should point to this descriptor
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x0			; segment base bits 0-15
	db 0x0			; base 16-23 bits
	db 0b10011011	; access byte
	db 0b11101111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits

; offset 0x10 (16 bytes)
data_descriptor:	; ds, es, fs, gs, and ss should point to this descriptor
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x0			; segment base bits 0-15
	db 0x0			; base 16-23 bits
	db 0b10010011	; access byte
	db 0b11101111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits
; offset 0x18 (24 bytes)
stack_descriptor:
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x00			; segment base bits 0-15
	db 0x00			; base 16-23 bits
	db 0b10010011	; access byte
	db 0b11101111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits
end_of_gdt:
toc: 
	dw end_of_gdt - gdt - 1 	; limit (Size of GDT)
	dd gdt

[BITS 32]
pmodecont:
;set up BDA for DIY 486 motherboard
mov ax, 0x22
mov ebx, 0x30D0
mov [ebx], al

mov ebp, 0x30000
mov esp, ebp
mov		eax, 0x10	; set data segments to data selector (0x10)
mov		es, ax
mov		fs, ax
mov		gs, ax
mov		ds, ax

;=========================
;maybe the general purpose registers have to have segments in them but probably not
;this source mentions doing that so its not as far fetched as it sounds, but it probably still doesn't make a difference:
;http://www.c-jump.com/CIS77/ASM/Memory/lecture.html
mov 	ebx, eax
mov		ecx, eax
mov 	edx, eax
;=========================

mov		edi, eax
mov		esi, eax
mov 	ax, 0x18	;the stack gets its own segment now
mov		ss, ax

;copy the bios tom to location 0x10000 and then jmp to it
;out 0xE9, al
mov ebx, 0xFFFE0000
mov ecx, 0x10000
again:
mov al, [ebx]
mov [ecx], al
inc ebx
inc ecx
;mov dx, 0xE9
;out dx, al
cmp ebx, 0xFFFFFFFF
jne again
;mov dx, 0xE9
;out dx, al

jmp 08h:0x10000
hlt
times 0x1FF-($-$$)+1 db 0xFF