;all this stuff is in 32bit assembly so don't put any real mode specific stuff in here
[BITS 32]
[CPU 486]
section .system_api
extern system_call_entry    ;get the extern "C" void system_call_entry() function from c++

;unsigned int systemCall(unsigned int)
systemCall:
    ;pop eax
	jmp system_call_entry   ;just get inside the c++ code section
ret

entryAddress dq 0x100000

global configure_entry_point
configure_entry_point:
    mov al, [0x30D0]
    and al, 01000000b
    mov cl, 6
    ror al, cl

    cmp al, 0
    jne highramentry

    lowramentry:
        mov eax, 0x10400
        mov [entryAddress], eax
    ret
    highramentry:
        mov eax, 0x100400
        mov [entryAddress], eax
    ret

;test the system call functionality within c++ os without actually having to load a c++ program from disk into ram
global system_call_test
system_call_test:
    ;push ebp
	;mov ebp, esp
	
	;jmp 08h:0x100400	;change to wherever the global system call entry point is
    ;push eax
    ;mov eax, [entryAddress]
    jmp [entryAddress]
	;;leave
    ;jmp system_call_entry
    ;leave
ret
