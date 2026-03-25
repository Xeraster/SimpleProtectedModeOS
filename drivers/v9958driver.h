#ifndef V9958_H
#define V9958_H

//benchark idea https://github.com/paul-reiners/quadrillionth-decimal-place

//palette registers are tricky dicky
extern "C" void v9958_writePaletteRegister(char registerNumber, char firstpalettebyte, char secondpalettebyte);

//this one should work as-is
extern "C" void v9958_writeStandardRegister(char registerNumber, char newValue);

//*run this from c++ to get a more reliably unoptimized and consistently slow delay loop. delays for the amount of time its supposed to delay for when the wait feature (register 25) is disabled (and it has to be disabled because if enanled, write cycles won't work on the current verilog)
extern "C" void v9958_compatibilityDelay();

extern "C" void v9958_clearVram();

//faster than clearVram, use for TEXT2 mode
extern "C" void v9958_clearText();

extern "C" void v9958_setupDefaultColors();

extern "C" void v9958_rowsColumnsToCursorPos(short rownum, short colnum);

extern "C" void v9958_RowsColumnsToVram(short rownum, short colnum);

//extern "C" void v9958_ShiftScreenUp();
//void v9958_rowsColumnsToCursor(short r, short c);

//rows have to be kept track in of in here now
short rownum = 0;
short colnum = 0;

void init9958Video();

void setup9958Text2Mode()
{
    init9958Video();

    v9958_clearText();

    v9958_rowsColumnsToCursorPos(rownum, colnum);
}

//for the first few tests, c++ os will only be given control of the v9958 after it has fonts written and wont have the ability to reload them
void VdpCharsIntoRam()
{
    v9958_writeStandardRegister(14, 0);
    v9958_compatibilityDelay();
    outb(0x00, 0xA021);
    v9958_compatibilityDelay();
    outb(0x51, 0xA021);
    v9958_compatibilityDelay();
    for (int i = 0; i < 2048; i++)
    {
        outb(vgafont8[i], 0xA020);
        
        //waste a LITTLE BIT of time
        int p = 0;
        for (int y = 0; y = 200; y++)
        {
            p++;
        }
    }
}

//this one has to be rewritten in c++ bcause it writes fonts to vram and what that code does isn't compatible with this
//This subroutine initializes various registers on the v9958 to set up for use in text mode. It sets the text mode to TEXT 2, configures the VRAM, and sets up other related registers.
void init9958Video()
{
    v9958_clearVram();

    VdpCharsIntoRam();

    //vdpCharsIntoRam
    v9958_compatibilityDelay();
    v9958_writeStandardRegister(0, 0x04);//set the screen mode flag for text2 mode
    v9958_compatibilityDelay();
    v9958_writeStandardRegister(1, 0x54);//set more screen mode flags for text2 mode
    v9958_compatibilityDelay();
    v9958_writeStandardRegister(7, 0xF0);//set the text color to 15 and set background color to 0
    v9958_compatibilityDelay();
    v9958_writeStandardRegister(12, 0x0A);//rtfm to find out what this does
    v9958_compatibilityDelay();
    v9958_writeStandardRegister(13, 0x44);//rtfm to find out what this does
    v9958_compatibilityDelay();
    v9958_writeStandardRegister(2, 0x33);//rtfm to find out what this does
    v9958_compatibilityDelay();
    v9958_writeStandardRegister(4, 0x32);//rtfm to find out what this does
    v9958_compatibilityDelay();
    v9958_writeStandardRegister(3, 0x27);//rtfm to find out what this does
    v9958_compatibilityDelay();
    v9958_writeStandardRegister(10, 0x0);//rtfm to find out what this does
}

void v9958_ShiftScreenUp()
{
    v9958_compatibilityDelay();
    v9958_writeStandardRegister(14, 0);
    v9958_compatibilityDelay();
    outb(0xA0, 0xA021);
    v9958_compatibilityDelay();
    outb(0x00, 0xA021);

    //store the contents of the screen before scroll up
    char* aSpot = (char*)malloc(0x760); //2048 - 160 = 1888d or 760h
    for (int i = 0; i < 1888; i++)
    {
        aSpot[i] = inb(0xA020);
    }

    //there, all the data currently on the screen except for the topmost 160 bytes is now in ram probably
	//reset vram pointer to the beginning of vram
    v9958_writeStandardRegister(14, 0);
    v9958_compatibilityDelay();
    outb(0x00, 0xA021);
    v9958_compatibilityDelay();
    outb(0x20, 0xA021);

    //write all that saved data back into ram
    for(int i = 0; i < 1888; i++)
    {
        outb(aSpot[i], 0xA020);
    }
    //now erase the bottom part of the screen by writing zeros to it
    for (int i = 0; i < 160; i++)
    {
        outb(0x00, 0xA020);
    }

}

//the all important char printing code
void v9958_printChar(unsigned char input)
{
    v9958_rowsColumnsToCursorPos(rownum, colnum);
    asmOutb(input, 0xA020);

    //everytime a char is printed, update the counter
    colnum++;
    if (colnum > 80)
    {
        colnum = 0;
        rownum++;
    }


}

void v9958_newline()
{
    rownum++;
    colnum = 0;
    if (rownum < 23)
    {
        v9958_ShiftScreenUp();
        rownum -= 2;
    }

    v9958_RowsColumnsToVram(rownum, colnum);
    v9958_rowsColumnsToCursorPos(rownum, colnum);

}

/*void v9958_rowsColumnsToCursor(short r, short c)
{

}*/

#endif