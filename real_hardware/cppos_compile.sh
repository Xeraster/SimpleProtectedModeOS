#!bin/bash

#NOTE: you need a C++ OS binary that has been compiled for the target system in this firectory and it needs to be named "disk.bin"
cd ../
sh cross-buildLinkScript-alternate.sh
cp disk.bin "real_hardware/bios.bin"
cd "real_hardware"
nasm cppos_bootloader.asm -o cppos_bootloader.bin
nasm cppos_bootloader_secondstage.asm -o cppos_bootloader_secondstage.bin
cat cppos_bootloader.bin >> cppos_bootloader_secondstage.bin
fallocate bios.bin -l 130304 -x    #1FFFF - 2FF
cat cppos_bootloader_secondstage.bin >> bios.bin
#done. flash bios.bin to rom
#minipro -p AM29F010@DIP32 -w bios.bin
