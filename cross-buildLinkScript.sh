g++ -c -march=i486 -mtune=i486 -m32 -mfpmath=387 -fno-pie -nodefaultlibs -nostdlib -fno-exceptions -fno-stack-protector -O1 -o system.o system.cpp

nasm -g -f elf32 -o bootloader.o bootloader.asm
nasm -g -f elf32 -o misc_tools.o misc_tools.asm

g++ -w -march=i486 -mtune=i486 -m32 -mfpmath=387 -fno-pie -nodefaultlibs -nostdlib -O1 -fstack-usage -o disk.bin -T link.ld system.o misc_tools.o bootloader.o
# add -lgcc to the end of that last command if getting errors
#simply replace g++ with the location of your cross compiler's g++ executable. I replaced that with g++ in the github repo for readability reasons
#note that it doesn't seem to make a difference on real hardware or emulators if you use a cross compiler but I want to get this working on a 486 pc which is why all those parameters are in there

#copy the disk image to the real hardware directory
#rm "real hardware/disk.bin"
#cp disk.bin "real hardware/disk.bin"
#cd "real hardware"
#sh bootloaderInstall.sh
#don't need any of that anymore. C++ OS now gets loaded off of a FAT32 filesystem using C++ Bootloader
#rename to sys.bin, the file the bootloader looks for

#10/30/24 explaination:
#there is a config file somewhere configured to point to fat32test.bin. This config file is connected to whatever program has the mdel command
#running the following 3 lines puts the system kernel into that fat32 drive image. It does not update the bootloader, the bootloader is in a different file. so if you fuck up the bootloader you're fucked
cp disk.bin sys.bin
mdel c:sys.bin
mcopy sys.bin c:
