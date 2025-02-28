;all this stuff is in 32bit assembly so don't put any real mode specific stuff in here
[BITS 32]
[CPU 486]
section .auxfunctions

;sets IOPL to 3 regardless of anything else
global forceIOPL_High
forceIOPL_High:
    pushf
    pop eax
    or eax, 0x3000
    push eax
    popf
ret

;sets IOPL to 0 regardless of anything else
global forceIOPL_Low
forceIOPL_Low:
    pushf
    pop eax
    and eax, 0xCFFF
    push eax
    popf
ret

;here's my real mode x86 keyboard stuff. I see no reason why it wouldn't work here tbh
global waitAck
waitAck:
        call kbd8042WaitReadReady
        in al, 060h
        cmp al, 0FAh
        jne waitAck

ret

global kbd8042WaitReadReady
kbd8042WaitReadReady:
    in al, 064h                    ;8042 status port
    and al, 00000001b                   ;read the data register read ready bit
    cmp al, 1
    jne kbd8042WaitReadReady             ;if it's not ready, keep waiting until it is
ret

global kbd8042WaitWriteReady
kbd8042WaitWriteReady:
    in al, 064h
    and al, 00000010b
    ror al, 1
    cmp al, 0
    jne kbd8042WaitWriteReady
ret

;do note that changing compilers may make these stop working because of calling conventions and all
;char byte, unsigned int port
global asmOutb
asmOutb:
    push ebp
    mov ebp, esp

    mov edx, [ebp+12]
    mov eax, [ebp+8]
    out dx, al

    leave

ret

;unsigned int port
global asmInb
asmInb:
    push ebp
    mov ebp, esp
    
    mov edx, [ebp+8]

    mov eax, 0
    in al, dx

    leave
ret

;;the same thing but for 16 bit transfers
global asmOutW
asmOutW:

    push ebp
    mov ebp, esp

    mov edx, [ebp+12]
    mov eax, [ebp+8]
    out dx, ax

    leave

ret

global asmInW
asmInW:
    push ebp
    mov ebp, esp
    
    mov edx, [ebp+8]

    mov eax, 0
    in ax, dx

    leave
ret

;cpuid command. get information about the cpu and then return it
global cpu_ident
cpu_ident:
    push ebp
    mov ebp, esp

    CPU 586
    mov eax, 0x01
    cpuid
    CPU 486

    ;info we want is in the eax register

    leave
ret

;get the cpu vendor
global cpu_vendor
cpu_vendor:
    push ebp
    mov ebp, esp

    CPU 586
    mov eax, 0
    cpuid
    CPU 486

    ;good enough
    mov eax, ebx

    leave
ret

global enable_A20
enable_A20:
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

;a really basic way to move an error message to a location in memory so that it can be read with a debugger
generic_error:
    mov al, 'F'
    mov ah, 0x04
    mov [0xB8936], ax
    mov al, 'U'
    mov ah, 0x04
    mov [0xB8938], ax
    mov al, 'C'
    mov ah, 0x04
    mov [0xB893A], ax
    mov al, 'K'
    mov ah, 0x04
    mov [0xB893C], ax

    hlt

ret

;drop into real mode and jump to the 1mb ram boundary. Takes no inputs because i'm not going to spend an entire afternoon trial and erroring stack calling conventions until it works
bootFromHighMem:
ret

;trigger interrupt 3 and hopefully trigger an arbitrary breakpoint
global insert_breakpoint_asm
insert_breakpoint_asm:
    push ebp
    mov ebp, esp
    int 3
    leave
ret

Invalid_TSS:

ret

;sets up the ldt and then fucks off
setupldt:

    ;put address of error subroutine
    mov eax, generic_error
    mov [Invalid_TSS_INT], ax
    mov cl, 16
    sar eax, cl
    mov [Invalid_TSS_INT+6], ax

    lidt [top_idt] ;load gdt settings
ret

;Now for the idt
;you need interrupts to work since exceptions are interrupts
idt:

    dq 0x00
    Invalid_TSS_INT:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT1:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT2:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT3:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT4:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT5:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT6:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT7:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT8:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT9:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT10:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT11:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT12:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT13:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT14:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT15:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
    Invalid_TSS_INT16:
	;each entry is 64 bits (damn lol what a pain)
	dw 0x00	;offset bits 0-15
	dw 0x10	;segment selector 16 bits
	dw 0xEE00	;bits32-39 reserved. bits 40-43 gate type. bit 45-46 dpl. bit 47 present bit
	dw 0x00	;offset bits 16-31
end_of_idt:
top_idt:
	dw end_of_idt - idt - 1
	dd idt

    %include "drivers/vgadriver.asm"