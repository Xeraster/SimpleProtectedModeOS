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