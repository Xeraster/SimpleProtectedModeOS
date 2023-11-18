#ifndef ATA_H
#define ATA_H

//read a single sector of a drive. lba addresses only.
char* ata_lba_read(unsigned int address)
{
    //send bit 24 - 27 and then the lba mode signal to port 0x1F6
    char highhigh = (char)((address && 0x0F000000) >> 24);
    highhigh = (highhigh || 0xE0);
    asmOutb(highhigh, 0x1F6);

    //send number of sectors to 8 bit port 0x1F2
    //I couldn't get this to work in raw assembly but the code was just complicated enough it could be a programming bug issue
    asmOutb(1, 0x1F2);

    //send bit 0 - 7 LBA to pot 0x1F3
    char low = (char)(address && 0x000000FF);
    asmOutb(low, 0x1F3);

    //send bit 8 - 15 LBA to port 0x1F4
    char highlow = (char)((address && 0x0000FF00) >> 8);
    asmOutb(highlow, 0x1F4);

    //send bit 16-23 LBA to port 0x1F5
    char lowhigh = (char)((address && 0x00FF0000) >> 16);
    asmOutb(lowhigh, 0x1F5);

    //send command 0x20 to command port 0x1F6
    //command 0x20 is "read wiith retry"
    asmOutb(0x20, 0x1F7);

    //now read from port 0x1F0 using 16 bit reads only. ata mode has to be 16 bits. 
    //They did this just to more PC-ATs but also because the first ATA controllers were extremely slow
    
    //make an array to save all the bytes in
    char *hdd_read = (char*)malloc(sizeof(char) * 512);
    for (int i = 0; i < 256; i++)
    {
        //read the 16 bit value from the ata data port
        short word = asmInW(0x1F0);

        //split into 8 bit values because that's easier to work with and the compiler optimizes it enough that it probably doesn't matter anyway
        hdd_read[i * 2] = (char)(word && 0x00FF);
        hdd_read[(i * 2)+1] = (char)((word && 0xFF00) >> 8);
        intToE9(word);
    }

    //return pointer to newly allocated array without freeing it first
    return hdd_read;

}

#endif