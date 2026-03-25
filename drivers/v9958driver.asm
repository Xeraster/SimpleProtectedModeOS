[BITS 32]
[CPU 486]

;brought over from the test bios
;cx needs to contain the number of wait loops
longDelay:
    mov bx, 0
    longDelayContinue:
    inc bx
    cmp bx, cx
    jne longDelayContinue
ret

;run this from c++ to get a more reliably unoptimized and consistently slow delay loop
global v9958_compatibilityDelay
v9958_compatibilityDelay:
    mov cx, 0xFFF
    call longDelay
ret

;x86 v9958 driver

;port #0 VRAM Data (R/W)		$A020
;port #1 Status register (R) / VRAM Address (W) / Register set-up (W)	$A021
;port #2 Palette registers (W)	$A022
;port #3 Register indirect addressing (W)
;$2FFE - used for draw rectangle filled function
col: dw 0x0000
row: dw 0x0000

init9958Video:
	;the FIRST THING TO DO is to set bit 2 of register 25 to hopefully enable wait state insertion and for it to work better on x86
	;;mov al, 25
	;;mov bl, 00000100b
	;;call VdpWriteToStandardRegister
	;remove this if it causes problems
	;edit: causes problems with the way the chipset currently handles the SPECIAL and EXACT timing of the v9958 write cycles. read cycles need no fpga magic

    call v9958_clearVram             ;doesnt work for some reason
    call VdpCharsIntoRam
	mov cx, 0FFFh
	call longDelay
    call v9958_setupDefaultColors

	mov cx, 0FFFh
	call longDelay

    ;activate TEXT 2 mode
    mov al, 0
    mov bl, 0b00000100 		;change to %00000100 for text2. %00000000
    call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay

    ;set up register 1
    mov al, 1
	mov bl, 0b01010000
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay

    mov bl, 0b11110000 	;register 7. bits 7-4 = text color in text modes. bits 3-0 = screen backdrop color. uses 16 color values
	mov al, 7
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay

	;set up register 12 and configure cursor color in text 2 mode
	mov bl, 0b00001010
	mov al, 12
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay

	;configure register 13, the cursor blink time register
	mov bl, 0b01000100
	mov al, 13
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay

    ;set values of pattern generator, pattern layout and pattern color table
	;I am using "MSX system default" values
	;text 2 pattern generator: 01000h-017FFh. Pattern layout: 00000h-0077Fh (00000h-0086Fh in 26.5 line mode). Pattern color table 800h-8EFh (800h-90Dh in 26.5 mode)
	mov bl, 0b00000011
	mov al, 2
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay

	mov bl, 0b00000010
	mov al, 4
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay

	mov bl, 0b00100111
	mov al, 3
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay

	mov bl, 0b00000000
	mov al, 10
	call VdpWriteToStandardRegister

ret

;text 2 pattern generator: 01000h-017FFh. Pattern layout: 00000h-0077Fh (00000h-0086Fh in 26.5 line mode)
VdpCharsIntoRam:
ret

;sets the color palette to the default
;I'm making it the same as the Microsoft Windows default 16-color palette
;https://en.wikipedia.org/wiki/List_of_software_palettes#Microsoft_Windows_default_16-color_palette
global v9958_setupDefaultColors
v9958_setupDefaultColors:
    ;to use the palette registers on an x86 system:
	;bl register needs to contain palette register number you want to write to (0-15)
    ;al register needs to contain first palette byte
    ;ah register needs to contain second palette byte
    ;note that the pointer value in register 16 auto increments each time you do this

	;color 0 = black
	mov bl, 0
	mov al, 0
	mov ah, 0
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time


	;color 1 = maroon
	mov bl, 1
	mov al, 0b01000000
	mov ah, 0b00000000
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 2 = dark green
	mov bl, 2
	mov al, 0b00000000
	mov ah, 0b00000100
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 3 = poop brown
	mov bl, 3
	mov al, 0b01000000
	mov ah, 0b00000100
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 4 = navy blue
	mov bl, 4
	mov al, 0b00000100
	mov ah, 0b00000000
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 5 = purple
	mov bl, 5
	mov al, 0b01000100
	mov ah, 0b00000000
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 6 = teal
	mov bl, 6
	mov al, 0b00000100
	mov ah, 0b00000100
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 7 = silver
	mov bl, 7
	mov al, 0b01000100
	mov ah, 0b00000100
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 8 = gray
	mov bl, 8
	mov al, 0b00100010
	mov ah, 0b00000010
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 9 = red
	mov bl, 9
	mov al, 0b01110000
	mov ah, 0b00000000
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 10 = bright green
	mov bl, 10
	mov al, 0b00000000
	mov ah, 0b00000111
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 11 = yellow
	mov bl, 11
	mov al, 0b01110000
	mov ah, 0b00000111
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 12 = blue
	mov bl, 12
	mov al, 0b00000111
	mov ah, 0b00000000
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time

	;color 13 = fuchsia
	mov bl, 13
	mov al, 0b01110111
	mov ah, 0b00000000
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay

	;color 14 = aqua
	mov bl, 14
	mov al, 0b00000111
	mov ah, 0b00000111
	call VdpWriteToPaletteRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time


	;color 15 = white
	mov bl, 15
	mov al, 0b01110111
	mov ah, 0b00000111
	call VdpWriteToPaletteRegister

ret

;sets up and configures graphics 4 mode
;	pattern layout (bitmap): 00000h-069ffh
;	sprite patterns 07800h-07fffh
;	sprite attributes 07600h-0767Ffh
;	sprite colors 07400h-075ffh
setupG4Mode:

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time
	;put the vdp into graphics mode 4. m5 = 0. m4 = 1. m3 = 1. m2 = 0. m1 = 0
	;register 0
	mov bl, 00000110b 		;change to %00000100 for text2. %00000000
	mov al, 0
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time
	;set up register 1
	mov bl, 01000000b
	mov al, 1
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time
	;set up register 8
	mov bl, 00001000b
	mov al, 8
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time
	;set register 23 to zero
	mov bl, 0
	mov al, 23
	call VdpWriteToStandardRegister

	call clearMostVram

	;here's what I need to set:
	;	pattern layout (bitmap): 00000h-069ffh
	;	sprite patterns 07800h-07fffh
	;	sprite attributes 07600h-0767Ffh
	;	sprite colors 07400h-075ffh

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time
	;pattern layout table
	mov bl, 00011111b
	mov al, 2
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time
	;sprite patterns
	mov bl, 00001111b
	mov al, 6
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time
	;sprite attributes high
	mov bl, 00000000b
	mov al, 11
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time
	;sprite attributes low ($7600)
	mov bl, 11101111b
	mov al, 5
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time
	;sprite color table high
	mov bl, 00000001b
	mov al, 10
	call VdpWriteToStandardRegister

	mov cx, 0FFFh
	call longDelay		;palette register stuff seems to need extra time
	;sprite color table low
	mov bl, 11010000b
	mov al, 3
	call VdpWriteToStandardRegister

ret

;bl register needs to contain palette register number you want to write to (0-15)
;al register needs to contain first palette byte
;ah register needs to contain second palette byte
;note that the pointer value in register 16 auto increments each time you do this
global v9958_writePaletteRegister
v9958_writeToPaletteRegister:
    push ebp
    mov ebp, esp

    mov edx, [ebp+8]     ;reg number
    mov ebx, [ebp+12]    ;first palette byte
    mov eax, [ebp+16]    ;second palette byte

    ;make bl contain the palette register number
    mov ecx, ebx
    mov bl, dl
    mov edx, eax
    ;now. ecx contains the first palette byte and edx contains the second palette byte
    mov al, cl
    mov ah, dl
    ;that should do it

	push ax
	push bx
        ;bl alredy contains register number
        mov al, 16
		call VdpWriteToStandardRegister
	pop bx
	pop ax  ;BUG! was "pop bx"

	mov cx, 0FFFh
    call longDelay		;palette register stuff seems to need extra time

    ;somehow this is even more annoying than the z80 version
    mov dx, 0xA022
    out dx, al
    mov al, ah
    out dx, al

    leave

ret

VdpWriteToPaletteRegister:
	push ax
	push bx
        ;bl alredy contains register number
        mov al, 16
		call VdpWriteToStandardRegister
	pop bx
	pop ax  ;BUG! was "pop bx"

	mov cx, 0FFFh
    call longDelay		;palette register stuff seems to need extra time

    ;somehow this is even more annoying than the z80 version
    mov dx, 0xA022
    out dx, al
    mov al, ah
    out dx, al

ret

;bl should contain register data
;al should contain register number
global v9958_writeStandardRegister
v9958_writeStandardRegister:
    push ebp
    mov ebp, esp

    mov edx, [ebp+8]     ;reg number
    mov eax, [ebp+12]    ;value to set register to
    
    mov bl, al ;register data into bl
    mov al, dl ;register number into al

    mov dx, 0xA021

	;write the data byte first because that's just what you do
    push ax
        mov al, bl
        out dx, al

		mov cx, 0FFh
		call longDelay
    pop ax

	;write the register number next
	;add a, 128
	or al, 10000000b		;different way of adding 128 to al
	out dx, al              ;dx should still be set to 0xA021

    leave
ret

;bl should contain register data
;al should contain register number
VdpWriteToStandardRegister:
    mov dx, 0xA021

	;write the data byte first because that's just what you do
    push ax
        mov al, bl
        out dx, al

		mov cx, 0FFh
		call longDelay
    pop ax

	;write the register number next
	;add a, 128
	or al, 10000000b		;different way of adding 128 to al
	out dx, al              ;dx should still be set to 0xA021
ret

;clear first 8kb of vram
global v9958_clearVram
v9958_clearVram:
	mov al, 14
	mov bl, 0
	call VdpWriteToStandardRegister
	mov dx, 0xA021		;it has to be port 1 not port 0
	mov cx, 02FFh
	call longDelay
	mov al, 0
	out dx, al
	mov cx, 02FFh
	call longDelay
	add al, 64
	out dx, al
	mov cx, 02FFh
	call longDelay

	mov dx, 0xA020	;port 0 is data port, riiight..??
	mov bx, 0x2000 	;clear the first 8kb of vram
	mov al, 0

	clearContinue:
		out dx, al
        dec bx
        cmp bx, 0
        jne clearContinue

        ;i dont miss this crap
		;dec hl
		;ld a, h
		;or l
		;nop
		;nop
		;jr nz, clearContinue

ret

;clear the forst 2048 bytes of ram
global v9958_clearText
v9958_clearText:
	mov cx, 0x3FF
	call longDelay
	mov al, 14
	mov bl, 0
	call VdpWriteToStandardRegister
	mov cx, 0x3FF
	call longDelay
	mov dx, 0xA021		;it has to be port 1 not port 0
	mov al, 0
	out dx, al
	mov cx, 0x3FF
	call longDelay
	add al, 64
	out dx, al

	mov dx, 0xA020	;port 0 is data port, riiight..??
	mov bx, 0x7FF 	;clear the first 2048 bytes of ram
	mov al, 0

	clearTextContinue:
		out dx, al
        dec bx
        cmp bx, 0
        jne clearTextContinue
		
ret

;clear first 8kb of vram
clearMostVram:
	mov al, 14
	mov bl, 0
	call VdpWriteToStandardRegister
	mov dx, 0xA021		;it has to be port 1 not port 0
	mov cx, 02FFh
	call longDelay
	mov al, 0
	out dx, al
	mov cx, 02FFh
	call longDelay
	add al, 64
	out dx, al
	mov cx, 02FFh
	call longDelay

	mov dx, 0xA020	;port 0 is data port, riiight..??
	mov bx, 0x8000 	;clear the first 32kb of vram
	mov al, 0

	clearMostVramContinue:
		out dx, al
        dec bx
        cmp bx, 0
        jne clearMostVramContinue

        ;i dont miss this crap
		;dec hl
		;ld a, h
		;or l
		;nop
		;nop
		;jr nz, clearContinue

ret

RowsColumnsToVramTempVar: dw 0x0000		;use this for temporary variable storage within RowsColumnsToVram
global v9958_RowsColumnsToVram
v9958_RowsColumnsToVram:
	push ebp
    mov ebp, esp

    mov edx, [ebp+8]     ;short rownum
    mov eax, [ebp+12]    ;short colnum
	mov [es:row], dx
	mov [es:col], ax

	mov al, 14
    mov bl, 0                           ;i dont remember what this does
    call VdpWriteToStandardRegister

	mov al, [es:row]		;get the current row amount
	mov cl, al
	mov al, 80
	mul cl				;rows * 80 = number of character spaces from
	mov cx, ax			;move the result of this multiplication into cx for safekeeping

	mov ah, 0
	mov al, [es:col]		;get the current column number
	xchg ax, cx
	add ax, cx
	
	mov [es:RowsColumnsToVramTempVar], ax		;save that number for later
	
	;after this, ax contains cursor position in linear vram. converting this to vram address is going to be hard tho
    mov al, 14
	mov bl, 0		;text2 mode won't ever have a high enough number for this to be set to anything besides 0
	call VdpWriteToStandardRegister
    ;mov cx, 0FFh
    ;call longDelay
	mov dx, 0xA021
	mov ax, [es:RowsColumnsToVramTempVar]
	out dx, al								;lower 8 bits to address pointer register
    ;mov cx, 0FFh
    ;call longDelay
	mov ax, [es:RowsColumnsToVramTempVar]
	xchg ah, al
	and al, 00111111b						;the correct upper bits that are available
	add al, 64
	out dx, al

	;there, hopefully that will do it
	leave

ret

RowsColumnsToCursorPosVar0: dw 0x0000		;temporary variable storage for stuff involving this one function
RowsColumnsToCursorPosVar1: dw 0x0000		;temporary variable storage for stuff involving this one function
RowsColumnsToCursorPosDisable db 0x00				;trying to speed up a slow subroutine that's slow due to the slow ass v9958 and its retarded cursor shit? Setting this byte to anything except zero disables this subroutine
global v9958_rowsColumnsToCursorPos
v9958_rowsColumnsToCursorPos:
	push ebp
    mov ebp, esp

    mov edx, [ebp+8]     ;short rownum
    mov eax, [ebp+12]    ;short colnum
	mov [es:row], dx
	mov [es:col], ax

	;if the disable byte is not set to zero, exit
	mov al, [es:RowsColumnsToCursorPosDisable]
	cmp al, 0
	jne RowsColumnsToCursorPosGTFO

	call eraseCursorTable		;there should only ever be one cursor space

	mov ax, [es:row]			;load the row number from ram
	mov cl, 80
	mul cl						;ax <- al * cl 
	mov cx, ax					;save calculation result in cx for later

	mov ax, [es:col]
	add ax, cx											;the linear vram address based on rows cols variables should now be in ax

	;divide this number by 8
	;there are 8 cursor positions in one byte of vram
	mov cl, 8
	div cl					;fuck yeah x86 instruction set. Divide ax by 8. returns al = quotient. ah = remainder

	mov [es:RowsColumnsToCursorPosVar0], ax			;save result to ram for later
	;mov [es:RowsColumnsToCursorPosVar1], ax			;save result to ram for debugging
	;xchg ax, dx
	;mov [es:RowsColumnsToCursorPosVar1], ax			;save remainder to ram for later

	;set up the vram address pointer
	mov al, 14
	mov bl, 0			;for text2 mode, this will never need to be anything besides zero
	call VdpWriteToStandardRegister
    ;mov cx, 0FFh
    ;call longDelay
	mov ax, [es:RowsColumnsToCursorPosVar0]
	mov dx, 0xA021
	out dx, al			;whatever's in ax right now should be the correct thing for bits 7-0
    ;mov cx, 0FFh
    ;call longDelay
	;xchg ah, al			;get bits 15-8 into al
	;and al, 00111111b	;chop off the bits that need to be set to zero
	mov al, 00001000b	;cursor data starts at 0x800
	add al, 64			;next commaznd is data write so change bit 6 to a 1
	out dx, al

	;the vram address pointer should be ready now, all that's left is to write the cursor position byte into vram
	
	;at this point, ax + dx = the bit location to write the cursor bit to
	;or, ax is the byte location to write to and dx is the bit number
	mov ax, [es:RowsColumnsToCursorPosVar0]			;get the remainder from earlier
	xchg al, ah
	mov cl, al										;put remainder in cl register
	mov al, 0x80
	shr al, cl		;do basically the same thing the z80 code does to shift the bits to the correct position. I love this instruction so much because it makes things easier. You need at least a 286 to use it though.
	mov [es:RowsColumnsToCursorPosVar1], al

	;mov cx, 0FFh
    ;call longDelay		;this used to be required for it to work at all
	mov dx, 0xA020
	out dx, al

	RowsColumnsToCursorPosGTFO:

	leave

ret

;clear all the cursor stuff
eraseCursorTable:
	;set the vram address pointer
	mov al, 14
	mov bl, 0
	call VdpWriteToStandardRegister
	mov cx, 0FFh
    call longDelay
	mov dx, 0xA021
	mov al, 0
	out dx, al
	mov cx, 0FFh
    call longDelay
	add al, 01001000b			;copying from z80 bios, didn't stop to consider what this does
	out dx, al
	mov cx, 0FFh
    call longDelay

	mov bx, 0x00FE				;number of bytes containing cursor information
	mov dx, 0xA020
	mov al, 0					;completely clear every byte in question
	
	eraseCursorTableContinue:
		out dx, al
		dec bx
		cmp bx, 0
		jne eraseCursorTableContinue

	;ld a, 14
	;ld d, 0
	;call VdpWriteToStandardRegister
	;ld d, 0
	;out (c), d
	;ld d, %01001000
	;out (c), d
	;ld hl, $00FE
	;ld c, $20
	;ld e, 0
	;eraseCursorTableContinue:
	;	out (c), e
	;	dec hl
	;	ld a, h
	;	or l
	;	nop
	;	nop
	;	jr nz, eraseCursorTableContinue

ret
