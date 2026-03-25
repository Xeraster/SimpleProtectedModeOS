#!bin/bash

#first, copy the mbr bootstrap code area. the first 218 bytes. (modern mbr is 218 bytes, legacy is 446 bytes. Thanks, Obama)
dd if=disk.bin of=fat32test.bin conv=notrunc count=446 bs=1

#now, make a copy of disk.bin with the lower 512 bytes chopped off
dd if=disk.bin of=disk_trimmed.bin conv=notrunc skip=512 bs=1 iflag=skip_bytes

#now overwrite this into the disk image file
dd if=disk_trimmed.bin of=fat32test.bin conv=notrunc bs=1 seek=512

#clean up the trash (not useful to end user)
#rm disk_trimmed.bin
