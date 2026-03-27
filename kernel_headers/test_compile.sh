g++ -c -march=i486 -mtune=i486 -m32 -mfpmath=387 -fno-pie -nodefaultlibs -nostdlib -fno-exceptions -fno-stack-protector -O1 -o testprog.o testprog.cpp

nasm -g -f elf32 -o system.o system.asm

g++ -w -march=i486 -mtune=i486 -m32 -mfpmath=387 -fno-pie -nodefaultlibs -nostdlib -O1 -fstack-usage -o testprog.x86 -T program_link.ld testprog.o system.o
