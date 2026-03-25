#!bin/bash
#use this one when compiling real mode os to use it as a bios rom instead of a bootloader-loaded system

nasm -i ../ ../system.asm -o bios.bin
nasm -i ../ bootloader.asm -o bootloader.bin
#nasm padding.asm -o nasm_hack.bin

#cat bios.bin >> nasm_hack.bin
#bootloader goes in mbr
#bios.bin goes on the FAT32 filesystem somewhere
