;bios data area bytes D0-EF are free real estate. use it for stuff. BDA at 0x3000 seems like a good spot
;D0 bit 1-0 = installed video card. 0 = vga. 1 = v9958. 2 = ice 40/tgss, 3 = other
;D0 bits 2-4: reserved. Nothing right now, but expect for this to do something else in the future
;D0 bit 5: 0 if this system has a 8224 PIT, 1 if it has a ice 40 fpga microsecond timer
;D0 bit 6. if system entry is 0x10000, this is 0. if system entry is 0x100000, this is 1
;D0 bit 7. if 1, keyboard port is at 61. if 0, it's a "normal" ps2 keyboard setup
CPU 486
bits 32                 ;;this is all protected mode assembly since it gets loaded by a special bootloader
;I gave up trying to get syslinux to load this. I'm not saying it's easier to DIY this part yourself BUT I spent the same amount of time making my own bootloader as I did trying to get syslinux to work and I was only sucessful in one of those endeavours.

SECTION .bootloader

extern enable_A20
extern _Z5beginv

;this should be enough such that doing a jump to the location in ram where this stuff is causes stuff to get run
_start:
cli 	;disable interrupts because not doing this increases chances of things not working

;set up BDA for normal x86 pcs.
mov ax, 0x40
mov ebx, 0x30D0
mov [ebx], al

;just roll with whatever protected mode parameters are already in place except reset the stack
mov ebp, 0x30000
mov esp, ebp
call _Z5beginv

;print a bunch of garbage to make it really obvious if it ever actually gets to this part of the code
mov eax, 0B8000h
mov cl, 'F'
mov bx, 0
mov [eax], cl
inc eax
mov [eax], cl

;;enable A20 line using "fast" method
;in al, 0x92
;test al, 2
;jnz after
;or al, 2
;and al, 0xFE
;out 0x92, al
;after:

mov al, '3'
mov dx, 0xE9
out dx, al
call bootloader_enable_A20	;there. now the bootloader uses nothing outside its own file/binary/section. in theory this should make linking easier. in practice, it doesn't help anything

mov eax, 0B8002h
mov cl, 'F'
mov bx, 0
mov [eax], cl
inc eax
mov [eax], cl
inc eax

mov al, '4'
mov dx, 0xE9
out dx, al
;after doing all this protected mode stuff, attempt to jump into the c code
call _Z5beginv
;call 0x10000
hlt

bootloader_enable_A20:
        ;cli
 
        call    a20wait
        mov     al,0xAD
        out     0x64,al
 
        call    a20wait
        mov     al,0xD0
        out     0x64,al
 
        call    a20wait2
        in      al,0x60
        push    eax
 
        call    a20wait
        mov     al,0xD1
        out     0x64,al
 
        call    a20wait
        pop     eax
        or      al,2
        out     0x60,al
 
        call    a20wait
        mov     al,0xAE
        out     0x64,al
 
        call    a20wait
        ;sti
        ret
 
a20wait:
        in      al,0x64
        test    al,2
        jnz     a20wait
        ret
 
 
a20wait2:
        in      al,0x64
        test    al,1
        jz      a20wait2
        ret

; to fill this sector and make it bootable:
	manualStackAddress dd 0x00010000
   times 510-($-$$) db 90h
   dw 0AA55h
