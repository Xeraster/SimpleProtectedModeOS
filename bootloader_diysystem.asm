;works on qemu. works on bochs. doesn't work on a real life 486 system.
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
