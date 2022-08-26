#!bin/bash
#g++ -march=i486 -m32 -std=c++17 -nostartfiles -ffreestanding -nostdlib -nolibc -nodefaultlibs -Ttext 0x7e00 system.cpp -o system
g++ -march=i486 -m32 -std=c++17 -nostartfiles -ffreestanding -fPIE -Ttext 0x7e00 system.cpp -o system
#strip --strip-all system
objcopy -O binary -j .text system system.raw
objcopy -O binary -j .rodata system system_rodata.raw
#objcopy -O binary -j .data system system_data.raw
#objcopy -O binary -j .bss system system_bss.raw
#ld system -m elf_i386 -r -Tlink.ld
#now the nasm stuff
nasm bootloader.asm -o bootloader.bin

#append both files together. make the assembly binary at the front and the c compiled onbe be behind it
cat system.raw >> bootloader.bin
cat system_rodata.raw >> bootloader.bin
#cat system_data.raw > bootloader.bin
#cat system_bss.raw > bootloader.bin

#I couldn't get ld to link correctly or behave reasonably for anything. At this point, I don't believe it's actually possible to use ld with a nasm-compiled binary at all and while there may be complicated semi-reliable hacks accomplish this, there's especially no way to make it interface with compiled, protected mode c code the way I need it to be done
