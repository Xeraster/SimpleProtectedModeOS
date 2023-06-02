;works on qemu. works on bochs. doesn't work on a real life 486 system.
[CPU 486]
bits 16

SECTION .text

extern _Z5beginv

_start:
cli 	;disable interrupts because not doing this increases chances of things not working

;xor ax, ax    ; make sure ds is set to 0
mov eax, 0
mov ds, ax
cld
;begin comment out block (comment out everything between these 2 comments to make it runnable via dos, doesn't work but would've been great if it did because buying a cf card and only ever getting 1 full drive write out of it makes every recompilation really expensive)
; start putting in values:
;mov dl, 80h   ;select first drive;doing this makes bochs not able to load it.. because bochs has to load it as a floppy, makes sense
mov ah, 2h    ; int13h function 2
mov al, 55    ; we want to read a lot of sectors I guess. 55 * 512 = ~28kb = a lot 
mov ch, 0     ; from cylinder number 0
mov cl, 2     ; the sector number 2 - second sector (starts from 1, not 0)
mov dh, 0     ; head number 0
mov bx, 0h
mov es, bx    ; es should be 0
mov bx, 7e00h; 512bytes from origin address 7e00h
int 13h
;end comment out block

lgdt [toc] ;load gdt settings

;set cpu in protected mode before jumping to the protected mode c code
mov eax, cr0
;or eax, 00000000000000000000000000000001b
or eax, 0x01
mov cr0, eax

;jmp $+2 			;putting this here seems to at least make it get far enough to execute a hlt instruction once it gets to protected mode
jmp 08h:pmodecont

gdt:

; offset 0x0 (0 bytes)
null_descriptor:
	dq 0x0

; offset 0x8 (8 bytes)
code_descriptor:	; cs should point to this descriptor
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x0			; segment base bits 0-15
	db 0x0			; base 16-23 bits
	db 0b10011110	; access byte
	db 0b11111111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits

; offset 0x10 (16 bytes)
data_descriptor:	; ds, es, fs, gs, and ss should point to this descriptor
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x0			; segment base bits 0-15
	db 0x0			; base 16-23 bits
	db 0b10010010	; access byte
	db 0b11111111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits
; offset 0x18 (24 bytes)
stack_descriptor:
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x00			; segment base bits 0-15
	db 0x00			; base 16-23 bits
	db 0b10010010	; access byte
	db 0b11111111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits
end_of_gdt:
toc: 
	dw end_of_gdt - gdt - 1 	; limit (Size of GDT)
	dd gdt


[BITS 32]
pmodecont:
;maybe using the debug registers will make breakpoints work
;mov eax, dr7
;and eax, 11111111111100001111111111111111b
;mov dr7, eax

;mov eax, 0x89EC ;realloc is at 89EC
;mov dr0, eax

;hlt
;movzx esp, [ds:manualStackAddress]
mov ebp, 0x2FFFF
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
;in al, 64h

;push ax
;pop ax
;hlt
;mov eax, 0x07e00
;call eax

;print a bunch of garbage to make it really obvious if it ever actually gets to this part of the code
mov eax, 0B8000h
mov cl, 'F'
mov bx, 0
mov [eax], cl
inc eax
mov [eax], cl

;;enable A20 line using "fast" method
in al, 0x92
test al, 2
jnz after
or al, 2
and al, 0xFE
out 0x92, al
after:

mov eax, 0B8002h
mov cl, 'F'
mov bx, 0
mov [eax], cl
inc eax
mov [eax], cl
inc eax

;after doing all this protected mode stuff, attempt to jump into the c code
jmp _Z5beginv
hlt

; to fill this sector and make it bootable:
	manualStackAddress dd 0x00010000
   times 510-($-$$) db 90h
   dw 0AA55h
