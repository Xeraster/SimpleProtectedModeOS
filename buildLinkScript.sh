#!bin/bash
#g++ -march=i486 -m32 -std=c++17 -nostartfiles -ffreestanding -nostdlib -nolibc -nodefaultlibs -Ttext 0x7e00 system.cpp -o system
g++ -march=i486 -m32 -std=c++17 -nostartfiles -ffreestanding -fPIE -Ttext 0x7e00 system.cpp


nasm -f elf32 -g -F dwarf bootloader.asm

ld -m elf_i386 -o bootloader.bin bootloader.o -Ttext 0x7e00 --oformat=binary
ld -m elf_i386 -o system.raw system -Tlink.ld
#append both files together. make the assembly binary at the front and the c compiled onbe be behind it
#cat system.raw >> bootloader.bin
#cat system_rodata.raw >> bootloader.bin
#cat system_data.raw > bootloader.bin
#cat system_bss.raw > bootloader.bin

#I couldn't get ld to link correctly or behave reasonably for anything. At this point, I don't believe it's actually possible to use ld with a nasm-compiled binary at all and while there may be complicated semi-reliable hacks accomplish this, there's especially no way to make it interface with compiled, protected mode c code the way I need it to be done
