[BITS 32]
[CPU 486]
extern begin
;offset 0x0. the beginning of the program
call configure_entry_point	;might as well do it now
jmp begin

entryAddress dq 0x100000	;store the system call entry address in this byte here

;VERY IMPORTANT: run this in your userspace program FIRST before doing ANY SYSTEM CALLS
global configure_entry_point
configure_entry_point:
	;;check bios data area (starting at 0x30D0) bit 6 to see if the system entry point is 0x10000 or 0x100000
    mov al, [0x30D0]
    and al, 01000000b
    mov cl, 6
    ror al, cl

    cmp al, 0
    jne highramentry

	;system call is always at an offset of 0x400 from the system entry point
    lowramentry:
        mov eax, 0x10400
        mov [entryAddress], eax
    ret
    highramentry:
        mov eax, 0x100400
        mov [entryAddress], eax
    ret

global systemCall
;unsigned int systemCall(unsigned int)
systemCall:
	jmp [entryAddress]
ret