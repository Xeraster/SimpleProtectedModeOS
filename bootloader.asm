[CPU 586]
org 7c00h

SECTION text

global _start
_start:
xor ax, ax    ; make sure ds is set to 0
mov ds, ax
cld
; start putting in values:
mov ah, 2h    ; int13h function 2
mov al, 50    ; we want to read a lot of sectors I guess. 50 * 512 = ~25kb = a lot 
mov ch, 0     ; from cylinder number 0
mov cl, 2     ; the sector number 2 - second sector (starts from 1, not 0)
mov dh, 0     ; head number 0
xor bx, bx    
mov es, bx    ; es should be 0
mov bx, 7e00h ; 512bytes from origin address 7e00h
int 13h

cli 	;disable interrupts because not doing this increases chances of things not working
;mov ax, 0
;mov gs, ax
lgdt [toc] ;load gdt settings

;set cpu in protected mode before jumping to the protected mode c code
mov eax, cr0
;or eax, 00000000000000000000000000000001b
or eax, 0x01
mov cr0, eax

jmp $+2 			;putting this here seems to at least make it get far enough to execute a hlt instruction once it gets to protected mode
jmp 08h:pmodecont

gdt:

; offset 0x0 (0 bytes)
null_descriptor:
	dq 0x0

; offset 0x8 (8 bytes)
code_descriptor:	; cs should point to this descriptor
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x0			; base first 0-15 bits
	db 0x0			; base 16-23 bits
	db 0b10011010	; access byte
	db 0b11001111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits

; offset 0x10 (16 bytes)
data_descriptor:	; ds, es, fs, gs, and ss should point to this descriptor
	dw 0xffff		; segment limit first 0-15 bits
	dw 0x0			; base first 0-15 bits
	db 0x0			; base 16-23 bits
	db 0b10010010	; access byte
	db 0b11001111	; high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x0			; base 24-31 bits
end_of_gdt:
toc: 
	dw end_of_gdt - gdt - 1 	; limit (Size of GDT)
	dd gdt


[BITS 32]
pmodecont:
;hlt
movzx esp, sp
mov ebp, esp
mov		ax, 0x10	; set data segments to data selector (0x10)
mov		ss, ax
mov		es, ax
mov		fs, ax
mov		gs, ax
mov		ds, ax
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
inc eax
mov [eax], cl
inc eax
mov [eax], cl
inc eax
mov [eax], cl
inc eax
mov [eax], cl
inc eax
mov [eax], cl
inc eax
mov [eax], cl
inc eax
mov [eax], cl
inc eax

;;enable A20 line using "fast" method
in al, 0x92
test al, 2
jnz after
or al, 2
and al, 0xFE
out 0x92, al
after:

;after doing all this shit, attempt to jump into the c code
jmp 0x00007e00
hlt

; to fill this sector and make it bootable:
   times 510-($-$$) db 0 
   dw 0AA55h
