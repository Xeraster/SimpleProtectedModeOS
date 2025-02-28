![Alt text](screenshot.png?raw=true "a screenshot of the system when it boots")

to build, use "cross-buildLinkScript.sh" then run the disk.bin file using "qemu-system-i386 -drive file=fat32test.bin,format=raw,index=0"

You need to install a compatible bootloader which is an entire project itself.

There is no actual cross compiler because no way of running externally loaded programs from within C++ OS has been devised yet.

Works well on bochs or qemu without errors. Works on real hardware most of the time. 

mtools can be difficult sometimes. On Ubuntu and Void Linux, the included .mtoolsrc file usually works but on Fedora you have to add the "partition=1" parameter at the end. As of 01/31/2025 this is what the example .mtoolsrc file contains: 

` drive c: file="fat32test.bin" partition=1 `

## tl;dr
sh cross-buildLinkScript.sh generates a "sys.bin" file and tries to put it inside a FAT32 MBR formatted drive image. On my personal development machine, this is a MBR FAT32 drive image with a specially made bootloader inserted into the first ~65kb worth of dummy sectors on the image.

# Commands
- help. The help command. It's more of a minigame but if you "win", you get a list of functions (it used to provide no help information at all).

- clear. clears the screen
- mount. mounts an ATA drive. Only works on motherboards with legacy ATA for now. Only works for MBR. ex: "mount 0" mounts the first partition of ATA disk 0. FAT32 is the only one tha works. I had started working on FAT16 but didn't finish it, so FAT16 won't work.
- cd. change directory. ex: "cd STUFF"
- ls. list everything in te current directory. The same as the linux "ls" command just without all the special options.
- colortest. prints each of the 16 mode 3h displayable colors
- cpuinfo. run the cpu id test and print the results.
- mbrinfo. prints information about the detected MBR
- read. reads a location in memory. The input numbers have to be in hex. ex: read 100000
- write. writes to ram. ex: "write 100000 FF" writes 0xFF to memory location 0x100000
- in. reads a byte from an i/o port. ex: "in 3f4"
- out. writes a byte to an i/o port. ex: "out 3f4 66"
- view. loads a plaintext file and prints the content. ex: "view README"
- gview. loads a RAW 256 color image file, switches to a 320x200 video mode and displays the picture. Only works on some vga cards and doesn't have the ability to switch back to vga 3h mode. It works on qemu and probably bochs.
- vgainfo. a command that facilitates the development of vga drivers.
- gtest. switches to a 320x200 graphics 256 color mode and draws a basic test. Doesn't have the ability to switch back to vga 3h mode
- findvesa. searches the contents of low ram and prints every occurance it found of a VESA header.
- fpgavideotest. a special development command used only for the fpga video card I am developing. Some of this command's options may crash or freeze the system if run without that video card installed.
- md5. hashes a given string into md5. Works most of the time.
- aes. encrypts a given string using AES. works most of the time.
- substringtest. a test for finding out how well the memory manager is working.
- mem. prints memory allocation information. I'm not sure how correct it is.
- pcicheck. a placeholder for a command that checks for the presence of pci and prints the results. Doesn't work right now.
- mbrinfoset. I forgot what this does.
- aestest. I forgot what this does.
- atatest. I forgot what this does.
- fatload. I forgot how to use this.

# System requirements
It doesn't work on a 286 because you'd have to use open watcom and the gcc linking toolchain isn't compatible with open watcom so there's no practical way to do this. but if someone figures it out that would be funny.
I haven't ever tried to compile with 386 instructions but c++ os needs a floating point unit because it has a few built-in float functions. So I never bother to go lower than 486dx. Perhaps 486sx and 386 can be done with minimal modifications, but I've never tried it.
- a 486dx or 486sx or better
- a motherboard with a ps/2 controller/legacy keyboard support (some motherboards passthrough detected usb keyboards on a 8042 compatibility layer)
- a motherboard with legacy ATA support
- recommended at least 256kb of ram (128kb program size, 64kb for stack, 64kb for working system memory)

# Compliation requirements
- gcc/g++
- nasm
- dd
- mtools

Run the "sh cross-buildLinkScript.sh" to compile. You will end up with a file called sys.bin. If mtools is properly configured and is pointing to a properly formatted FAT32 image fat32test.bin, it will put this file at the root of the first valid filesystem it finds in fat32test.bin.

# Usage.
Unlike with Real Mode OS, installing/running C++ OS is more difficult.

For running on qemu, the steps are as follows:
- Step 1. Use gparted to format a data medium as MBR with a FAT32 partition. I normally do this on a 2GB cfcard. Use dd to make a copy of the drive image and put it on your local filesystem somewhere.
- Step 2. Compile and install C++ Bootloader (a different project) to the newly created drive image. OR devise a way to use syslinux with this (I never got it to work with syslinux)
- Step 3. Once C++ Bootloader is installed, put the compiled sys.bin file at the root of the FAT32 partition.
- Step 4. run the drive image in qemu using "qemu-system-i386 -drive file=fat32test.bin,format=raw,index=0". You can also use dd to copy fat32test.bin to a CF card and boot it on hardware.

# Outstanding issues
- There is currently no way to run compiled programs within C++ OS.
- the cursor text insertion doesn't work that well
- the only filesystem support it has, FAT32, is read-only.
