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

extern _Z5beginv

;this should be enough such that doing a jump to the location in ram where this stuff is causes stuff to get run
_start:
cli 	;disable interrupts because not doing this increases chances of things not working
;out 0xE9, al
;just roll with whatever protected mode parameters are already in place except reset the stack
mov ebp, 0x37000
mov esp, ebp
out 0xE9, al
call _Z5beginv
hlt

; to fill this sector and make it bootable:
	manualStackAddress dd 0x00030000
   times 510-($-$$) db 90h
   dw 0AA55h
