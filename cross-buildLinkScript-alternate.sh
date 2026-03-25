g++ -c -march=i486 -mtune=i486 -m32 -mfpmath=387 -fno-pie -nodefaultlibs -nostdlib -fno-exceptions -fno-stack-protector -O1 -o system.o system.cpp

nasm -g -f elf32 -o bootloader.o bootloader_diysystem.asm
nasm -g -f elf32 -o misc_tools.o misc_tools.asm

g++ -w -march=i486 -mtune=i486 -m32 -mfpmath=387 -fno-pie -nodefaultlibs -nostdlib -O1 -fstack-usage -o disk.bin -T link_alternate.ld system.o misc_tools.o bootloader.o