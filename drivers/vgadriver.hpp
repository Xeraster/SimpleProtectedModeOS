/*;note: host system is supposed to store the following vga values at the following ram locations:
;   449 = current video mode
;   44A = number of columns on screen
;   44C = length of active screen buffer
;   44E = start of current page
;   450 = cursor positions
;   460 = cursor mode
;   462 = current display page
;   463 = base host address of port
;   465 = current mode
;   466 = current color
;   484 = number of character rows
*/

//maybe see about doing vesa resolutions 

//relies on up to date vga mode information being in ram at 449h
void resOfVGAMode(int *x, int *y)
{
    char mode = *(char *)0x449;

    if (mode == 3)          //16 color 80x25 text mode
    {
        *x = 80;
        *y = 25;
    }
    else if (mode == 0xD)   //16 color 320x200 graphics mode
    {
        *x = 320;
        *y = 200;
    }
    else if (mode == 0x10)  //16 color 640x350 graphics mode
    {
        *x = 640;
        *y = 350;
    }
    else if (mode == 0x12)  //16 color 640x480 graphics mode
    {
        *x = 640;
        *y = 480;
    }
    else if (mode == 0x13)  //256 color 320x200 graphics mode
    {
        *x = 320;
        *y = 200;
    }

    return;
}

char getvgamode()
{
    return *(char *)0x449;
}

void writeToVGAPort(short portNum, unsigned int index, char data)
{
    
}

unsigned char readFromVGAPort(short portNum, unsigned int index)
{
    //write the index
    //asmOutb(index, 0x3C0);

    //read the register
    //return asmInb(0x3C1);      //the vga index ports are *usually* 1 less than their corresponding data port
}
#define VGA_SQUENCER_ADDRESS 0x03C4
#define VGA_SEQUENCER_DATA 0x03C5
#define VGA_GRAPHICS_CONTROL_ADDRESS 0x03CE
#define VGA_GRAPHICS_CONTROL_DATA 0x03CF
#define VGA_CTRC_ADDRESS 0x3D4
#define VGA_CTRC_DATA 0x3D5
#define VGA_ATTRIBUTE_WRITE 0x3C0
#define VGA_ATTRIBUTE_READ 0x3C1
#define VGA_MISC_OUTPUT_REG_READ 0x3CC
#define VGA_MISC_OUTPUT_REG_WRITE 0x3C2
#define PEL_ADDRESS_READ 0x3C7
#define PEL_ADDRESS_WRITE 0x3C8
#define PEL_DATA_REG 0x3C9
//writes seem to work, reads rarely work but they sometimes do
char attributeRegisterRead(unsigned int index)
{
    /*asmOutb(0x00, 0x3D4);
    asmOutb(index, VGA_ATTRIBUTE_WRITE);
    char fuck = asmInb(VGA_ATTRIBUTE_READ);     //i saw this work one fucking time and it never worked again. I never changed the code
    asmOutb(0x00, 0x3D4);
    return fuck;*/
    //asmOutb(0x00, 0x3D4);

    /*
    //i swear to fuck this worked one time, i stopped paying attention for 10 fucking minutes and now it isn't working anymore. FUCK
    //from https://wiki.osdev.org/VGA_Hardware#Port_0x3C0:
    //To read the contents, feed the index into port 0x3C0
    asmOutb(index, 0x3C0);

    //then read the value from 0x3C1
    char pleaseWork = asmInb(0x3C1);    //"in" means read WHY THE FUCK IS IT CRASHING??? FUUUCK!!!

    //then read 0x3DA as it is not defined whether the VGA expects a data byte or index byte next
    //(those were the instructions on osdev, verbaitum)
    asmOutb(0x00, 0x3D4);
    return pleaseWork;*/

    //that stuff was completely wrong. lets try this:
    ///http://www.osdever.net/FreeVGA/vga/vgareg.htm
    //1.Input a value from the Input Status #1 Register (normally port 3DAh) and discard it. 
    /*asmInb(0x3DA);

    //2. Read the value of the Address/Data Register and save it for step 7.     
    char shit = asmInb(VGA_ATTRIBUTE_WRITE);

    //3. Output the index of the desired Data Register to the Address/Data Register 
    asmOutb(index, VGA_ATTRIBUTE_WRITE);

    //4. Read the value of the Data Register and save it for later restoration upon termination, if needed. 
    char data = asmInb(VGA_ATTRIBUTE_READ);

    //5. If writing, modify the value read in step 4, making sure to mask off bits not being modified. (im not writing anything)
    //6. If writing, write the new value from step 5 to the Address/Data register. (im not writing anything)

    //7. Write the value of Address register saved in step 1 to the Address/Data Register. 
    asmOutb(shit, VGA_ATTRIBUTE_WRITE);

    //8. If you wish to leave the register waiting for an index, input a value from the Input Status #1 Register (normally port 3DAh) and discard it
    asmInb(0x3DA);*/
    //nope, didnt work either

    //i swear to fuck this is the biggest load of fucking bullshit
    //asmInb(0x3DA);

    //DONT FUCKING TOUCH. DONT EVEN EDIT COMMENTS. DONT EVEN PUT YOUR CURSOR ANYWHERE NEAR THIS
    asmInb(0x3DA);
    asmOutb((char)index, VGA_ATTRIBUTE_WRITE);
    asmInb(0x3DA);
    //asmInb(0x3DA);
    char fuck = asmInb(VGA_ATTRIBUTE_READ);     //i saw this work one fucking time and it never worked again. I never changed the code
    asmInb(0x3DA);
    asmOutb(0x20, VGA_ATTRIBUTE_WRITE);
    return fuck;
    //return 0x00;

    //it's too complicated to do in assembly
    //return vga_readPaletteReg((char)index); 
}

void attributeRegisterWrite(unsigned int index, char data)
{
    asmOutb(index, VGA_ATTRIBUTE_WRITE);
    asmOutb(data, VGA_ATTRIBUTE_WRITE);
}

char crtcRegisterRead(unsigned int index)
{
    asmOutb((char)index, VGA_CTRC_ADDRESS);
    return asmInb(VGA_CTRC_DATA);
}

void crtcRegisterWrite(unsigned int index, char data)
{
    asmOutb((char)index, VGA_CTRC_ADDRESS);
    asmOutb(data, VGA_CTRC_DATA);
}

char sequencerRegisterRead(unsigned int index)
{
    //asmInb(VGA_SQUENCER_ADDRESS);
    asmOutb((char)index, VGA_SQUENCER_ADDRESS);
    //asmInb(VGA_SQUENCER_ADDRESS);
    return asmInb(VGA_SEQUENCER_DATA);
}

void sequencerRegisterWrite(unsigned int index, char data)
{
    //asmInb(VGA_SQUENCER_ADDRESS);
    asmOutb((char)index, VGA_SQUENCER_ADDRESS);
    //asmInb(VGA_SQUENCER_ADDRESS);
    asmOutb(data, VGA_SEQUENCER_DATA);
}

char miscOutputRegisterRead()
{
    return asmInb(VGA_MISC_OUTPUT_REG_READ);
}

void miscOutputRegisterWrite(char data)
{
    asmOutb(data, VGA_MISC_OUTPUT_REG_WRITE);
}

char graphicsControlRegisterRead(unsigned int index)
{
    asmOutb(index, VGA_GRAPHICS_CONTROL_ADDRESS);
    return asmInb(VGA_GRAPHICS_CONTROL_DATA);
}

void graphicsControlRegisterWrite(unsigned int index, char data)
{
    asmOutb(index, VGA_GRAPHICS_CONTROL_ADDRESS);
    asmOutb(data, VGA_GRAPHICS_CONTROL_DATA);
}

//writes 18 bit color data to the PEL register or the provided index
void PEL_write(unsigned char index, unsigned char red, unsigned char green, unsigned char blue)
{
    //set the index
    asmOutb(index, PEL_ADDRESS_WRITE);

    //write 18 bits of color data to the DAC
    asmOutb(0x3F&red, PEL_DATA_REG);
    asmOutb(0x3F&green, PEL_DATA_REG);
    asmOutb(0x3F&blue, PEL_DATA_REG);
}

//writes 18 bit color data to the PEL register of whatever index is already loaded and set
void PEL_write_autoincrement(unsigned char red, unsigned char green, unsigned char blue)
{
    //write 18 bits of color data to the DAC
    asmOutb(0x3F&red, PEL_DATA_REG);
    asmOutb(0x3F&green, PEL_DATA_REG);
    asmOutb(0x3F&blue, PEL_DATA_REG);
}

//clears the crtc protection bit
void unprotectCRTC()
{
    char original = crtcRegisterRead(0x11);
    original = (original & 0x7F);
    crtcRegisterWrite(0x11, original);

    original = crtcRegisterRead(0x03);
    original = (original & 0x7F);
    crtcRegisterWrite(0x03, original);
}

//sets the crtc protection bit
void protectCRTC()
{
    char original = crtcRegisterRead(0x11);
    original = (original | 0x80);
    crtcRegisterWrite(0x11, original);

    original = crtcRegisterRead(0x03);
    original = (original | 0x80);
    crtcRegisterWrite(0x03, original);
}

//get all the information about the mode specific registers so I can learn how to make my own graphics modes without real mode interrupts
void printVgaModeInfo(char info)
{
    //attribute registers
    if (info == 0 || (info & 0x08) > 0)
    {
        printString("Mode-specific vga attribute registers:", 0x0F);
        consoleNewLine();
        printString("Mode control: ", 0x0F);
        print8BitHexInt(attributeRegisterRead(0x10));
        consoleNewLine();

        printString("Overscan register: ", 0x0F);
        print8BitHexInt(attributeRegisterRead(0x11));
        consoleNewLine();

        printString("Color plane enable: ", 0x0F);
        print8BitHexInt(attributeRegisterRead(0x12));
        consoleNewLine();

        printString("Horizontal panning: ", 0x0F);
        print8BitHexInt(attributeRegisterRead(0x13));
        consoleNewLine();

        printString("Color select: ", 0x0F);
        print8BitHexInt(attributeRegisterRead(0x14));
        consoleNewLine();
        consoleNewLine();
    }

    //sequencer registers
    if (info == 0 || (info & 0x01) > 0)
    {
        printString("Sequencer registers: ", 0x0F);
        consoleNewLine();

        printString("Clock mode register: ", 0x0F);
        print8BitHexInt(sequencerRegisterRead(0x01));
        consoleNewLine();

        printString("Character select: ", 0x0F);
        print8BitHexInt(sequencerRegisterRead(0x03));
        consoleNewLine();

        printString("Memory Mode: ", 0x0F);
        print8BitHexInt(sequencerRegisterRead(0x04));
        consoleNewLine();
    }

    if (info == 0 || (info & 0x02) > 0)
    {
        printString("Mode reg: ",0x0F);
        print8BitHexInt(graphicsControlRegisterRead(0x05));
        consoleNewLine();

        printString("Miscellaneous reg: ",0x0F);
        print8BitHexInt(graphicsControlRegisterRead(0x06));
        consoleNewLine();
    }

    if (info == 0 || (info & 0x10) > 0)
    {
        printString("Misc output reg: ",0x0F);
        print8BitHexInt(miscOutputRegisterRead());
        consoleNewLine();
    }

    printString("Press any key that generates a scancode and sends it through the ps/2 controller to continue.", 0x0F);
    waitChar();//it can be literally any key. Anything that generates a scancode
    consoleNewLine();
    consoleNewLine();

    if (info == 0 || (info & 0x04) > 0)
    {
        printString("CRTC registers: ", 0x0F);
        consoleNewLine();
        printString("Horizontal total: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x00));
        consoleNewLine();

        printString("H dslpy enable end: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x01));
        consoleNewLine();

        printString("H blnk strt: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x02));
        consoleNewLine();

        printString("H blnk end: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x03));
        consoleNewLine();

        printString("H rtrc strt: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x04));
        consoleNewLine();

        printString("H rtrc end: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x05));
        consoleNewLine();

        printString("Vertical total: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x06));
        consoleNewLine();

        printString("Overflow reg: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x07));
        consoleNewLine();

        printString("Preset row scan: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x08));
        consoleNewLine();

        printString("Max scan line: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x09));
        consoleNewLine();

        // crtc register indexes 0A-0F are cursor related. skipping them is not a typo.
        printString("V rtrc strt: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x10));
        consoleNewLine();

        printString("V rtrc end: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x11));
        consoleNewLine();

        printString("V dsply enble end: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x12));
        consoleNewLine();

        printString("Logical width: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x13));
        consoleNewLine();

        printString("undrln lctn: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x14));
        consoleNewLine();

        printString("V blnk strt ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x15));
        consoleNewLine();

        printString("V blnk end: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x16));
        consoleNewLine();

        printString("Mode control: ", 0x0F);
        print8BitHexInt(crtcRegisterRead(0x17));
        consoleNewLine();
    }
}

bool mode13hSetup()
{
    //turn off and unlock stuff, idk
    unsigned char poop = crtcRegisterRead(0x03);    //figure out if the user has to poop or not
    crtcRegisterWrite(0x03, (poop | 0x80));         //if the user has to poop, tell them where the nearest bathroom is
    poop = crtcRegisterRead(0x11);
    crtcRegisterWrite(0x11, (poop & ~0x80));        //if there is no bathroom available, open a shopping link for adult diapers in the browser

    //get all the attribute registers set to what osdev says they're supposed to be set to
    /*attributeRegisterWrite(0x10, 0x41);
    attributeRegisterWrite(0x11, 0x00);
    attributeRegisterWrite(0x12, 0x0F);
    attributeRegisterWrite(0x13, 0x00);
    attributeRegisterWrite(0x14, 0x00);*/
    vga_editPaletteReg(0x10, 0x41);
    vga_editPaletteReg(0x11, 0x00);
    vga_editPaletteReg(0x12, 0x0F);
    vga_editPaletteReg(0x13, 0x00);
    vga_editPaletteReg(0x14, 0x00);

    //the "misc output" register (that's really what it's called)
    miscOutputRegisterWrite(0x63);

    //sequencer registers
    sequencerRegisterWrite(0x00, 0x03);
    sequencerRegisterWrite(0x01, 0x0D);// was 1
    sequencerRegisterWrite(0x03, 0x0F);//was 0
    sequencerRegisterWrite(0x04, 0x0E);//was E
    //sequencerRegisterWrite(0x05, 0x06);//was E

    //graphics control registers
    graphicsControlRegisterWrite(0x05, 0x40);
    graphicsControlRegisterWrite(0x06, 0x05);
    graphicsControlRegisterWrite(0x07, 0x0F);
    graphicsControlRegisterWrite(0x08, 0xFF);

    unprotectCRTC();

    //crtc registers
    crtcRegisterWrite(0x00, 0x5F);
    crtcRegisterWrite(0x01, 0x4F);
    crtcRegisterWrite(0x02, 0x50);
    crtcRegisterWrite(0x03, 0x02);//was 82
    crtcRegisterWrite(0x04, 0x54);
    crtcRegisterWrite(0x05, 0x80);
    crtcRegisterWrite(0x06, 0xBF);
    crtcRegisterWrite(0x07, 0x1F);
    crtcRegisterWrite(0x08, 0x00);
    crtcRegisterWrite(0x09, 0x41);

    crtcRegisterWrite(0x10, 0x9C);
    crtcRegisterWrite(0x11, 0x0E);//was 8E
    crtcRegisterWrite(0x12, 0x8F);
    crtcRegisterWrite(0x13, 0x28);
    crtcRegisterWrite(0x14, 0x40);//was 40
    crtcRegisterWrite(0x15, 0x96);
    crtcRegisterWrite(0x16, 0xB9);
    crtcRegisterWrite(0x17, 0xE3);//was A3
    crtcRegisterWrite(0x18, 0xFF);

    protectCRTC();

    return true;
}

bool mode640Setup()
{
    //turn off and unlock stuff, idk
    unsigned char poop = crtcRegisterRead(0x03);    //figure out if the user has to poop or not
    crtcRegisterWrite(0x03, (poop | 0x80));         //if the user has to poop, tell them where the nearest bathroom is
    poop = crtcRegisterRead(0x11);
    crtcRegisterWrite(0x11, (poop & ~0x80));        //if there is no bathroom available, open a shopping link for adult diapers in the browser

    //get all the attribute registers set to what osdev says they're supposed to be set to
    /*attributeRegisterWrite(0x10, 0x41);
    attributeRegisterWrite(0x11, 0x00);
    attributeRegisterWrite(0x12, 0x0F);
    attributeRegisterWrite(0x13, 0x00);
    attributeRegisterWrite(0x14, 0x00);*/
    vga_editPaletteReg(0x10, 0x41);
    vga_editPaletteReg(0x11, 0x00);
    vga_editPaletteReg(0x12, 0x0F);
    vga_editPaletteReg(0x13, 0x00);
    vga_editPaletteReg(0x14, 0x00);

    //the "misc output" register (that's really what it's called)
    miscOutputRegisterWrite(0x63);

    //sequencer registers
    sequencerRegisterWrite(0x00, 0x03);
    sequencerRegisterWrite(0x01, 0x0D);// was 1
    sequencerRegisterWrite(0x03, 0x0F);//was 0
    sequencerRegisterWrite(0x04, 0x0E);//was E
    //sequencerRegisterWrite(0x05, 0x06);//was E

    //graphics control registers
    graphicsControlRegisterWrite(0x05, 0x40);
    graphicsControlRegisterWrite(0x06, 0x05);
    graphicsControlRegisterWrite(0x07, 0x0F);
    graphicsControlRegisterWrite(0x08, 0xFF);

    unprotectCRTC();

    //crtc registers
    crtcRegisterWrite(0x00, 0x5F);
    crtcRegisterWrite(0x01, 0x4F);
    crtcRegisterWrite(0x02, 0x50);
    crtcRegisterWrite(0x03, 0x02);//was 82
    crtcRegisterWrite(0x04, 0x54);
    crtcRegisterWrite(0x05, 0x80);
    crtcRegisterWrite(0x06, 0xBF);
    crtcRegisterWrite(0x07, 0x1F);
    crtcRegisterWrite(0x08, 0x00);
    crtcRegisterWrite(0x09, 0x41);

    crtcRegisterWrite(0x10, 0x9C);
    crtcRegisterWrite(0x11, 0x0E);//was 8E
    crtcRegisterWrite(0x12, 0x8F);
    crtcRegisterWrite(0x13, 0x28);
    crtcRegisterWrite(0x14, 0x40);//was 40
    crtcRegisterWrite(0x15, 0x96);
    crtcRegisterWrite(0x16, 0xB9);
    crtcRegisterWrite(0x17, 0xE3);//was A3
    crtcRegisterWrite(0x18, 0xFF);

    protectCRTC();

    return true;
}

//draw a pixel in 256 color mode
//256 color mode apparently doesn't use that bit plane bullshit, so 16 color vga mode will be entirely unsupported. Seriously. 256 color consumes less cpu time because it needs less code. WTF?
//you do not want to see what all it takes just to draw a pixel in 16 color graphics mode.
void drawPixel256(unsigned short x, unsigned short y, unsigned char color)
{
    //calculate offset
    unsigned int offset = (y*320) + x;

    //uuh, set this to 0? (there's only 64kb worth of pixels in 320x200x8 video mode)
    //asmOutb(0x00, 0x3CD);//change video bank. don't lose this, i think I deleted all the other occurances of this and its going to be hard to re-learn about if it gets forgotten

    //asmOutb(color, )
    //short dat = msg;
	//dat += (color<<8);
	*(char*)(0xA0000+offset) = color;
}

//takes a properly formatted image that's loaded in ram and copies it to the screen
void drawImage(void* data)
{
    for(int i = 0; i < 60000; i++)
    {
        *(char*)(0xA0000+i) = *(char*)(data+i);
    }
}

//draw a test pattern in 300x200 bullshit mode
void drawTestPattern256()
{
    unsigned char colorCount = 0;
    for (int i = 10; i< 266; i++)
    {
        //drawPixel256(40, i, 2);
        //*(char*)(0xA0000+i) = colorCount;
        bhm_line(40+i, 10, 40+i, 100, colorCount);
        colorCount++;
    }
}

//test the crtc functions to see if reading and writing works
bool vga_testcrtc()
{   
    unprotectCRTC();
    char originalValue = crtcRegisterRead(0x00);
    crtcRegisterWrite(0x00, 0x66);
    char result = crtcRegisterRead(0x00);
    crtcRegisterWrite(0x00, originalValue);     //put it back
    protectCRTC();

    if (result == 0x66)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool vga_testSequencer()
{
    char oldAddress = asmInb(VGA_SQUENCER_ADDRESS);
    //sequencerRegisterWrite(0x00, 0x00);
    //unprotectCRTC();
    char originalValue = sequencerRegisterRead(0x00);
    sequencerRegisterWrite(0x00, 0x66);
    char result = sequencerRegisterRead(0x00);
    sequencerRegisterWrite(0x00, originalValue);
    asmOutb(oldAddress, VGA_SQUENCER_ADDRESS);
    //protectCRTC();

    sequencerRegisterWrite(0x00, 0x03);
    
    if (result == 0x66)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//test the graphics control registers to see if readign and writing works
bool vga_testGraphicsControl()
{
    unprotectCRTC();
    char originalValue = graphicsControlRegisterRead(0x00);
    graphicsControlRegisterWrite(0x00, 0x66);
    char result = graphicsControlRegisterRead(0x00);
    graphicsControlRegisterWrite(0x00, originalValue);
    protectCRTC();
    
    if (result == 0x66)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//dumps the contents of every single vga register into this struct
void vga_videoMode :: dumpCurrentVideoMode()
{
    //read all the attribute registers. all of them.
    m_attributeSettings = (char*)malloc(0x15);
    for(int i = 0; i < 0x15; i++)
    {
        m_attributeSettings[i] = attributeRegisterRead(i);
    }

    m_crtcSettings = (char*)malloc(0x19);
    for (int i = 0; i < 0x19; i++)
    {
        m_crtcSettings[i] = crtcRegisterRead(i);
    }

    m_graphicsControlSettings = (char*)malloc(0x09);
    for (int i = 0; i < 0x09; i++)
    {
        m_graphicsControlSettings[i] = graphicsControlRegisterRead(i);
    }

    m_sequencerSettings = (char*)malloc(0x05);
    for (int i = 0; i < 0x05; i++)
    {
        m_sequencerSettings[i] = sequencerRegisterRead(i);
    }

    m_miscControl = miscOutputRegisterRead();
}

void vga_videoMode :: printRegisters(char info)
{
    // attribute registers
    if (info == 0 || (info & 0x08) > 0)
    {
        printString("attribute registers: ", 0x0F);
        consoleNewLine();
        for (int i = 0; i < 0x15; i++)
        {
            printInt(i, 0x0F, true);
            printChar(' ', 0x0F);
            printInt((m_attributeSettings[i] & 0xFF), 0x0F, true);
            consoleNewLine();
            if (i == 0x0F && info != 0x00)
            {
                printString("press any key to see the rest of the entries", 0x0F);
                waitChar();
                consoleNewLine();
            }
        }
    }

    //crtc
    if (info == 0 || (info & 0x04) > 0)
    {
        printString("crtc registers: ", 0x0F);
        consoleNewLine();
        for (int i = 0; i < 0x19; i++)
        {
            printInt(i, 0x0F, true);
            printChar(' ', 0x0F);
            printInt((m_crtcSettings[i] & 0xFF), 0x0F, true);
            consoleNewLine();
            if (i == 0x0F && info != 0x00)
            {
                printString("press any key to see the rest of the entries", 0x0F);
                waitChar();
                consoleNewLine();
            }
        }
    }

    //graphics controller
    if (info == 0 || (info & 0x02) > 0)
    {
        printString("graphic controller registers: ", 0x0F);
        consoleNewLine();
        for (int i = 0; i < 0x09; i++)
        {
            printInt(i, 0x0F, true);
            printChar(' ', 0x0F);
            printInt((m_graphicsControlSettings[i] & 0xFF), 0x0F, true);
            consoleNewLine();
        }
    }

    // sequencer registers
    if (info == 0 || (info & 0x01) > 0)
    {
        printString("sequencer registers: ", 0x0F);
        consoleNewLine();
        for (int i = 0; i < 0x05; i++)
        {
            printInt(i, 0x0F, true);
            printChar(' ', 0x0F);
            printInt((m_sequencerSettings[i] & 0xFF), 0x0F, true);
            consoleNewLine();
        }
    }

    printString("misc control register: ", 0x0F);
    printInt((char)m_miscControl, 0x0F, true);
}

//apply this video mode to the vga card
void vga_videoMode :: applyToSystem()
{
    //turn off and unlock stuff, idk
    unsigned char poop = crtcRegisterRead(0x03);
    crtcRegisterWrite(0x03, (poop | 0x80));
    poop = crtcRegisterRead(0x11);
    crtcRegisterWrite(0x11, (poop & ~0x80));

    //get all the attribute registers set to what osdev says they're supposed to be set to
    /*attributeRegisterWrite(0x10, 0x41);
    attributeRegisterWrite(0x11, 0x00);
    attributeRegisterWrite(0x12, 0x0F);
    attributeRegisterWrite(0x13, 0x00);
    attributeRegisterWrite(0x14, 0x00);*/

    for (int i = 0; i < 0x15; i++)
    {
        vga_editPaletteReg(i, m_attributeSettings[i]);
    }

    //the "misc output" register (that's really what it's called)
    miscOutputRegisterWrite(m_miscControl);

    //sequencer registers
    for (int i = 0; i < 0x05; i++)
    {
        sequencerRegisterWrite(i, m_sequencerSettings[i]);
    }


    for (int i = 0; i < 0x09; i++)
    {
        graphicsControlRegisterWrite(i, m_graphicsControlSettings[i]);
    }
    //graphics control registers

    unprotectCRTC();

    for (int i = 0; i < 0x19; i++)
    {
        // crtc registers
        if (i == 0x03)
        {
            crtcRegisterWrite(i, (m_crtcSettings[i] | 0x80));
        }
        else if (i == 0x11)
        {
            crtcRegisterWrite(i, (m_crtcSettings[i] & 0x80));
        }
        else
        {
            crtcRegisterWrite(i, m_crtcSettings[i]);
        }
    }

    protectCRTC();
}

// bresenham's line algorithm. Draw a line in 300x200 mode
void bhm_line(int x1, int y1, int x2, int y2, unsigned char color)
{
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1;
    dy = y2 - y1;
    dx1 = abs(dx);
    dy1 = abs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            x = x1;
            y = y1;
            xe = x2;
        }
        else
        {
            x = x2;
            y = y2;
            xe = x1;
        }
        drawPixel256(x, y, color);
        //DrawToSurface(x, y, theColor, surf);
        for (i = 0; x < xe; i++)
        {
            x = x + 1;
            if (px < 0)
            {
                px = px + 2 * dy1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    y = y + 1;
                }
                else
                {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            drawPixel256(x, y, color);
            //DrawToSurface(x, y, theColor, surf);
        }
    }
    else
    {
        if (dy >= 0)
        {
            x = x1;
            y = y1;
            ye = y2;
        }
        else
        {
            x = x2;
            y = y2;
            ye = y1;
        }
        drawPixel256(x, y, color);
        //DrawToSurface(x, y, theColor, surf);
        for (i = 0; y < ye; i++)
        {
            y = y + 1;
            if (py <= 0)
            {
                py = py + 2 * dx1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    x = x + 1;
                }
                else
                {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            //DrawToSurface(x, y, theColor, surf);
            drawPixel256(x, y, color);
        }
    }
}