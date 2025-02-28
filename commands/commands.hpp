bool parseCommand(string input)
{
    
    //help command
    //string helpQuery = input.substr(0,5);
    string temporary = input;
    printString(temporary, 0x0A);
    if (input.substr(0,4) == "help")
    //if (temporary.at(0) == 'h' && temporary.at(1) == 'e' && temporary.at(2) == 'l' && temporary.at(3) == 'p')
    {
        if (input == "help -please")
        {
            consoleNewLine(3);
            printString("Here is a list of some of the more useful commands.", 0x0F);
            consoleNewLine();
            printString("clear mount cd ls colortest cpuinfo mbrinfo read write in out view gview vgainfo vgadump gtest findvesa fpgavideotest md5 aes", 0x0F);
            consoleNewLine();
            return true;
        }
        else if (input.contains("-p"))
        {
            consoleNewLine(3);
            printString("no", 0x0F);
            return true;
        }
        else if (input.contains("-f"))
        {
            consoleNewLine(3);
            printString("Nice try, but the -f switch only does this", 0x0F);
            consoleNewLine(3);
            printString("maybe try saying please?", 0x0F);
            return true;
        }
        else if (input.contains("-h"))
        {
            consoleNewLine(3);
            printString("NOPE! YOU JUST GOT PRANKED! Try 'help -f' to see the help screen for real.", 0x0F);
            return true;
        }
        else
        {
            consoleNewLine(3);
            printString("help is for losers. Git gud noob. Try 'help -h' for more information", 0x0F);
            return true;
        }
        return true;

    }

    else if (temporary.substr(0,8) == "pcicheck")
    {
        consoleNewLine(3);
        pciCheck(temporary);
        return true;
    }

    else if (input == "mem")
    {
        //consoleNewLine(3);
        //usedMemory(input);
        printMemNum();
        return true;
    }

    else if (input == "clear")
    {
        clearScreen(3);
        setCurPos(0, 1);
        return true;
    }

    else if (input == "colortest")
    {
        colorTest(input);
        return true;
    }

    else if (input == "colortest256")
    {
        vga300200ShittyAssBullshit256.applyToSystem();

        for (int i = 0; i < 64000; i++)
        {
            *(char *)(0xA0000 + (i)) = 0;
        }

        for (int y = 0; y < 16; y++)
        {
            for (int x = 0; x < 16; x++)
            {
                unsigned int offset = (y*320) + x;
                *(char *)(0xA0000 + (offset)) = (y*16)+x;
            }
        }

        //drawTestPattern256();
        //bhm_line(100, 200, 50, 75, 3);
        //vgaDefaultTextmode.manual_delete();
        waitChar();
        vgaDefaultTextmode.applyToSystem();
        return true;
    }

    else if (input == "substringtest")
    {
        substringTest(input);
        return true;
    }

    else if (input == "cpuinfo")
    {
        printCpuInfo();
        return true;
    }

    else if (input == "debug1")
    {
        debugCommand1(input);
        return true;
    }

    else if (input == "debug2")
    {
        debugCommand2(input);
        return true;
    }

    else if (input == "debug3")
    {
        debugCommand3(input);
        return true;
    }

    else if (input == "debug4")
    {
        consoleNewLine();
        printString("video pointer = ", 0x0F);
        printInt((unsigned int)videoStart, 0x0F, true);
        return true;
    }

    else if (input == "debug5")
    {
        consoleNewLine();
        printString("Not a real command", 0x0F);
        return true;
    }

    else if (input == "memleak")
    {
        memLeak(1);
        return true;
    }

    else if (input == "meminfo")
    {
        consoleNewLine(3);
        memoryInfo(input);
        //consoleNewLine(3);
        return true;
    }

    else if (input.substr(0,10) == "mbrinfoset")
    {
        mbrInfoSetCommand(input.substr(10,input.length()));
        return true;
    }

    else if (input.substr(0,7) == "mbrinfo")
    {
        mbrInfoCommand(input);
        return true;
    }


    //md5 command. need help figuring out which version? the md5 command was added 10/18/2018. It gained actual functionality in 05/2024
    else if (input.substr(0,4) == "md5 ")
    {
        md5Test(temporary.substr(4,temporary.length()));
        return true;
    }

    //aec command to encrypt a user provided string. Added on 05/21/2024
    else if (input.substr(0,4) == "aes ")
    {
        aesTest(temporary.substr(4,temporary.length()));
        return true;
    }

    else if (input.substr(0,8) == "aestest ")
    {
        basicAesTest(temporary.substr(8,temporary.length()));
        return true;
    }

    else if (input.substr(0,7) == "atatest")
    {
        atatestCommand(input);
        return true;
    }

    else if (temporary.substr(0,5) == "read ")
    {
        readCommand(temporary.substr(5,temporary.length()));

        return true;
    }

    //else if (temporary.substr(0, 4) == "split")
    else if (temporary == "split")
    {
        stringSplitTest("todo");
        return true;
    }

    else if (input.substr(0,2) == "ls")
    {
        lsCommand(input);
        return true;
    }

    else if (input.substr(0,6) == "mount ")
    {
        unsigned int part = input.at(6) - 48;
        if (part < 0 | part > 9)
        {
            consoleNewLine();
            printString("Bad", 0x0F);
            return false;
        }

        mountCommand(part);
        return true;
    }

    else if (input.substr(0,3) == "cd ")
    {
        consoleNewLine();
        printString("before cd", 0x0F);
        cdCommand(input.substr(3, input.length()));
        printString("after cd", 0x0F);
        //printString("the cd command", 0x0F);
        return true;
    }

    else if (input.substr(0,5) == "size ")
    {
        fileSizeCommand(input.substr(5, input.length()));       //to save cpu cycles, my substr concatenates overruns so you dont have to waste memory and cpu cycles calculating the right length to avoid going over
        return true;
    }

    else if (input == "fatlba")
    {
        consoleNewLine();
        printString("LBA of first FAT = ",0x0F);
        printInt(FATLBA(&selectedVolume.fsinfo), 0x0F);
        return true;
    }

    else if (input.substr(0,8) == "fatload ")
    {
        consoleNewLine();
        fatLoadCommand(input.substr(8,input.length()));
        return true;
    }

    else if(input.substr(0,5) == "boot ")
    {
        consoleNewLine();
        boot32(input.substr(5,input.length()));
        return true;
    }

    else if(input.substr(0,5) == "view ")
    {
        consoleNewLine();
        viewCommand(input.substr(5,input.length()));
        return true;
    }

    else if(input.substr(0,6) == "gview ")
    {
        consoleNewLine();
        gViewCommand(input.substr(6,input.length()));
        return true;
    }

    else if (input.substr(0,6) == "pload ")
    {
        consoleNewLine();
        ploadCommand(input.substr(6,input.length()));
        return true;
    }

    else if(input.substr(0,7) == "vgainfo")
    {
        consoleNewLine();
        vgaInfoCommand(input.substr(7,input.length()));
        return true;
    }

    else if(input.substr(0,7) == "vgadump")
    {
        consoleNewLine();
        vgaDumpCommand(input.substr(7,input.length()));
        return true;
    }

    else if(input.substr(0,5) == "gtest")
    {
        consoleNewLine();
        gTestCommand(input);
        return true;
    }

    else if(input == "fontinfo")
    {
        consoleNewLine();
        fontInfo(input);
        return true;
    }

    else if(input == "findvesa")
    {
        findVesaCommand(input);
        return true;
    }

    //school assigment, I needed leverage of some kind so I attempted to impress my professor by showing my program is "so efficient" and "bigly" that it can run on a 486
    else if (input == "customerListTest")
    {
        //hahaha no
        consoleNewLine();
        customerListInit();
        consoleNewLine();
        return true;
    }

    else if (input.substr(0, 13) == "fpgavideotest")
    {

        consoleNewLine();
        string uinput;

        if (input.length() >= 15 && input[14] >= '0' && input[14] <= '9')
        {
            uinput = "";
            uinput += input[14];
        }
        else
        {
            //print a menu
            printString("1. Switch to vram display mode", 0x0F);
            consoleNewLine();
            printString("2. Switch to test pattern mode", 0x0F);
            consoleNewLine();
            printString("3. Fill vram with a solid color", 0x0F);
            consoleNewLine();
            printString("4. Print status", 0x0F);
            consoleNewLine();
            printString("5. Manual pixel write with parameters", 0x0F);
            consoleNewLine();
            printString("6. Same as option 3 but with parameters", 0x0F);
            consoleNewLine();
            printString("7. Checkerboard test pattern", 0x0F);
            consoleNewLine();
            printString("8. TGSS mode test", 0x0F);
            consoleNewLine();
            printString("9. TGSS manual text draw", 0x0F);
            consoleNewLine();
            printString("Please enter the number of your choice: ", 0x0F);
            uinput = waitString(true);
            consoleNewLine();
        }

        if (input.length() > 0)
        {
            char entry = uinput[0];
            if (entry == '1')
            {
                // dispaly contents of vram
                outb(0x70, 0x423);
                printString("switched to vram display mode", 0x0F);
            }
            else if (entry == '2')
            {
                // display the test pattern
                outb(0x60, 0x423);
                printString("switched to test pattern display mode", 0x0F);
            }
            else if (entry == '3')
            {
                fpgavideotestoption3helper(30000, 0xFFFF, 100000, 0);
                /*outb(0x00, 0x428);
                outb(0x00, 0x428);
                outb(0x00, 0x429);
                outb(0x00, 0x429);
                outb(0x00, 0x42A);
                outb(0x00, 0x42A);

                //write all the bullshit
                unsigned int timeoutCount = 0;
                unsigned int fullCount = 0;
                for(int i = 0; i < 30000; i++)
                {
                    unsigned char status = inb(0x426);
                    unsigned char full = status & 0x20;
                    int timeout = 0;
                    while (timeout < 100000 && status == 0xFF && full == 0x20)
                    {
                        timeout++;
                        status = inb(0x426);
                        full = status & 0x20;
                    }

                    if (timeout >= 100000)
                    {
                        timeoutCount++;

                        if (full == 0x20)
                        {
                            fullCount++;
                        }

                    }

                    outw(0xFFFF, 0x42C);
                }

                printString("wrote a bunch of bytes to port 0x42C starting at vram address 0x00", 0x0F);
                consoleNewLine();
                printString("timeoutCount = ", 0x0F);
                printInt(timeoutCount, 0x0F, false);
                consoleNewLine();
                printString("fullCount = ", 0x0F);
                printInt(fullCount, 0x0F, false);*/
            }
            else if (entry == '4')
            {
                char vstatus = inb(0x426);
                printString("register 0x426 = ", 0x0F);
                print8BitHexInt(vstatus);
            }
            else if (entry == '5')
            {
                printString("Number of pixels to write (max 255): ", 0x0F);
                string num = waitString(true);
                unsigned int numToWrite = stoi(num);
                if (numToWrite > 255)
                {
                    numToWrite = 255;
                }

                consoleNewLine();
                printString("Starting address (0 - 2^20-1): ", 0x0F);
                string address = waitString(true);
                unsigned int addressToWrite = stoi(address);

                if (addressToWrite > 1048576)
                {
                    addressToWrite = 1048576;
                }

                unsigned char lowByte = addressToWrite & 0xFF;
                unsigned char middleByte = (addressToWrite >> 8) & 0xFF;
                unsigned char highByte = (addressToWrite >> 16) & 0xFF;

                consoleNewLine();
                printString("Low byte = ", 0x0F);
                printInt(lowByte, 0x0F, true);
                printString(" Mid byte = ", 0x0F);
                printInt(middleByte, 0x0F, true);
                printString(" High byte = ", 0x0F);
                printInt(highByte, 0x0F, true);

                outb(lowByte, 0x428);
                outb(lowByte, 0x428);
                outb(middleByte, 0x429);
                outb(middleByte, 0x429);
                outb(highByte, 0x42A);
                outb(highByte, 0x42A);

                consoleNewLine();
                printString("What color (hex numbers only): ", 0x0F);
                string color = waitString(true);
                unsigned short colorNum = stoi(color, 0, 16);
                consoleNewLine();
                printString("insert delay? (Y/n): ", 0x0F);
                string option = waitString(true);
                bool doDelay;
                if (option[0] = 'y' || option[0] == 'Y')
                {
                    doDelay = true;
                }
                else
                {
                    doDelay = false;
                }

                for (int i = 0; i < numToWrite; i++)
                {
                    outw(colorNum, 0x42C);
                    if (doDelay)
                    {
                        for (int d = 0; d < 10000; d++)
                        {
                            int wastetime = 4;
                            wastetime = wastetime + 1;
                        }
                    }
                }
            }
            else if (entry == '6')
            {
                printString("How many pixels (30000): ", 0x0F);
                string option = waitString(true);
                unsigned int amount = 30000;
                if (option.length() > 2)
                {
                    amount = stoi(option, 0, 10);
                }
                consoleNewLine();
                printString("What color? (FFFF): ", 0x0F);
                string color = waitString(true);
                unsigned short colorNum = 0xFFFF;
                if (color.length() > 1)
                {
                    colorNum = stoi(color, 0, 16);
                }
                consoleNewLine();
                printString("How much timeout: ", 0x0F);
                string timeoutamt = waitString(true);
                unsigned int timeoutNum = 0;
                if (timeoutamt.length() > 1)
                {
                    timeoutNum = stoi(timeoutamt, 0, 10);
                }
                consoleNewLine();
                printString("How much delay: ", 0x0F);
                string delayamt = waitString(true);
                unsigned int delayNum = 0;
                if (delayamt.length() > 1)
                {
                    delayNum = stoi(delayamt, 0, 10);
                }
                printString("your settings: ", 0x0F);
                printInt(amount, 0x0F);
                printString(" ", 0x0F);
                printInt(colorNum, 0x0F);
                printString(" ", 0x0F);
                printInt(timeoutNum, 0x0F);
                printString(" ", 0x0F);
                printInt(delayNum, 0x0F);
                consoleNewLine();

                fpgavideotestoption3helper(amount, colorNum, timeoutNum, delayNum);
            }
            else if (entry == '7')
            {
                printString("Line length: ", 0x0F);
                string linestring = waitString(true);
                unsigned int linelen = 16;
                unsigned short color = 0xFFFF;
                int currentIt = 0;
                if (linestring.length() > 1)
                {
                    linelen = stoi(linestring, 0, 10);
                }
                consoleNewLine();

                outb(0x00, 0x428);
                outb(0x00, 0x428);
                outb(0x00, 0x429);
                outb(0x00, 0x429);
                outb(0x00, 0x42A);
                outb(0x00, 0x42A);

                // write all the bullshit
                unsigned int timeoutCount = 0;
                unsigned int fullCount = 0;
                for (int i = 0; i < 300000; i++)
                {
                    unsigned char status = inb(0x426);
                    unsigned char full = status & 0x70; //we're only concerned about bits 6, 5 and 4
                    int timeout = 0;
                    while (timeout < 1000 && (full != 0x40))
                    {
                        timeout++;
                        status = inb(0x426);
                        full = status & 0x70;

                        for (int d = 0; d < 20; d++)
                        {
                            int wastetime = 4;
                            wastetime = wastetime + 1;
                        }
                    }

                    if (timeout >= 1000)
                    {
                        timeoutCount++;

                        if (full != 0x40)
                        {
                            fullCount++;
                        }
                    }

                    outw(color, 0x42C);
                    currentIt++;
                    if (currentIt > linelen)
                    {
                        currentIt = 0;
                        if (color == 0x007F)
                        {
                            color = 0xFFFF;
                        }
                        else
                        {
                            color = 0x007F;
                        }
                    }

                }
            }
            else if (entry == '8')
            {
                //do the gtest command except on the fpga video card
                //drawPixel_tgss();
                fpgavideotestoption3helper(300000, 0x0000, 1000, 10);
                drawTestPattern_tgss();
                bhm_line_tgss(100, 200, 50, 75, 0xF000);

                unsigned int x = 0;
                unsigned int y = 0;
                for (int c = 32; c < 143; c++)
                {
                    //graphicsModeDrawText(x*8, y, c, 0x02, 0xFE, false);
                    graphicsModeDrawText_tgss(x*8, y, c, 0x07E0, 0x0000, false);
                    x++;
                    if (x*8 > 639)
                    {
                        x = 0;
                        y+=8;
                    }
                }
            }
            else if (entry == '9')
            {
                //get th information required to draw some text
                printString("String to print: ", 0x0F);
                string message = waitString(true);
                consoleNewLine();
                printString("X pos: ", 0x0F);
                string xStr = waitString(true);
                int x = stoi(xStr);
                consoleNewLine();
                printString("Y pos: ", 0x0F);
                string yStr = waitString(true);
                int y = stoi(yStr);
                consoleNewLine();

                printString("Color (4 digit hex values only): ", 0x0F);
                string color = waitString(true);
                consoleNewLine();
                unsigned short colorNum = 0xFFFF;
                if (color.length() > 1)
                {
                    colorNum = stoi(color, 0, 16);
                }

                printString("Background color (4 digit hex values only): ", 0x0F);
                string bkgcolor = waitString(true);
                consoleNewLine();
                unsigned short bkgcolorNum = 0xFFFF;
                if (bkgcolor.length() > 1)
                {
                    bkgcolorNum = stoi(bkgcolor, 0, 16);
                }

                //make sure the gpu is only written to when its ready
                unsigned char status = inb(0x426);
                unsigned char full = status & 0x70; //we're only concerned about bits 6, 5 and 4

                unsigned int ys = 0;
                unsigned int xs = 0;
                //now that we have the information, draw the text. not tested hopefully this works
                for (int c = 0; c < message.length(); c++)
                {
                    //wait until it equals 40 i think that will provide the clearest text
                    while(full != 0x40)
                    {
                        for (int d = 0; d < 100; d++)//100 loop cycles ought to be unnoticable and enough for text to work better
                        {
                            int wastetime = 4;
                            wastetime = wastetime + 1;
                        }

                        status = inb(0x426);
                        full = status & 0x70;
                    }

                    graphicsModeDrawText_tgss(x + (xs*8), y + ys, message.at(c), colorNum, bkgcolorNum, false);
                    xs++;
                    if (xs*8 > 639)
                    {
                        xs = 0;
                        ys+=8;
                    }
                }
            }
            else
            {
                printString("hey idiot, that was an invalid entry. Please enter 1, 2, 3 or 4 only.", 0x0F);
            }
        }
        else
        {
            printString("Error: you entered a string with a length of zero.", 0x0F);
        }

        return true;
    }

    else if (input.substr(0,2) == "in")
    {
        inCommand(input);
        return true;
    }

    else if (input.substr(0,3) == "out")
    {
        outCommand(input);
        return true;
    }

    else if (input.substr(0,5) == "write")
    {
        writeCommand(input);
        return true;
    }

    else if (input.substr(0,4) == "halt")
    {
        haltCommand(input);
        return true;
    }


    //reprint whatever the user typed. If memory allocation is broken even slightly, this will very obviously be wrong which makes it a great tool for troubleshooting
    //printString(temporary, 0x0D);

    //printString(input, 0x0E);
    //print32bitNumber(input, 1800);
    return false;
}

bool usedMemory(string input)
{
    //get amount of used dynamic memory
    //unsigned int usedMemory = 0;
	//for (int i = 0; i < memf.getSize(); i++)
	//{   
        //some cross compilers don't do nullptr
	//	usedMemory += getSizeOfMemBlock((void*)memf.at(i), nullptr);
        //usedMemory += getSizeOfMemBlock((void*)memf.at(i), 0);
	//}

    //printInt(usedMemory, 0x0F);
    printString("liballoc not supported",0x0F);
    return true;
}

bool pciCheck(string input)
{
    if (input.contains("-a"))
    {
        for (int i = 0; i < 10; i++)
        {
            char device = 0;

            //I am hardcoding the pci bus to zero because on qemu and the ms4144 socket 3 motherboard I have, bios interrupt 1A, BX=0101 reports "last pci bus index" to be zero despite having 3 pci slots
            unsigned int vendor = pciCheckVendor(0, i, &device);

            //now print that garbage to the screen
            printInt(i, 0x0E, true);
            printString(": Vendor: 0x", 0x0F);
            printInt(vendor, 0x0F, true);
            printString("   Device: 0x", 0x0F);
            printInt(device, 0x0F, true);
            consoleNewLine(3);
        }
    }
    else
    {
        //subtract the ascii value by 48. This means as long as the user only enters 0-9 in position index 10, it will work fine
        char checkSlot = (input[10])-48;
        char device = 0;

        //I am hardcoding the pci bus to zero because on qemu and the ms4144 socket 3 motherboard I have, bios interrupt 1A, BX=0101 reports "last pci bus index" to be zero despite having 3 pci slots
        unsigned int vendor = pciCheckVendor(0, checkSlot, &device);

        //now print that garbage to the screen
        printString("Vendor: 0x", 0x0F);
        printInt(vendor, 0x0F, true);

        consoleNewLine(3);
        printString("Device: 0x", 0x0F);
        printInt(device, 0x0F, true);
    }

    return true;

}

bool clearScreenCommand(string input)
{
    clearScreen(3);

    return true;
}

bool colorTest(string input)
{
    for (int i = 0; i < 16; i++)
    {
        consoleNewLine(3);
        printInt(i, 0x0F, true);
        printString(" color", i);
    }
    consoleNewLine(3);
    printString("vga mode: 3h text", 0x0F);
    consoleNewLine(3);
    return true;
}

//substring very obviosuly showcases any problems with memory allocation
bool substringTest(string input)
{
    //a panagram that teaches you a science lesson
    string testString1 = "A quart far of oil mixed with zinc oxide makes a very bright paint";
    string testString2 = testString1.substr(0, testString1.length() - 4);

    printString(testString2, 0x04);
    consoleNewLine(3);

    printString("paint jars.substr(0,6)=", 0x0F);
    string aTest = "paint jars";
    //printString("k", 0x0F);
    printString(aTest.substr(0, 6), 0x0F);
    printString(aTest, 0x0F);
    consoleNewLine(3);
    return true;
}

bool printCpuInfo()
{
    consoleNewLine(3);
    printString("cpuid=", 0x0D);

    int result = cpu_ident();
    printInt(result, 0x0F, true);
    consoleNewLine(3);
    printString("model=",0x0D);
    int model = (result >> 4) & 0xF;
    printInt(model, 0x0F, true);
    consoleNewLine(3);

    printString("family=",0x0D);
    int family = (result >> 8) & 0xF;
    printInt(family, 0x0F, true);
    consoleNewLine(3);

    printString("type=",0x0D);
    int type = (result >> 12) & 0x3;
    printInt(type, 0x0F, true);
    consoleNewLine(3);

    //printInt(getCpuVendor(), 0x0F, true);
    printString(cpuModelToString(getCpuVendor(), model, family), 0x0F);

    consoleNewLine(3);

    /*printString("extended model=",0x0D);
    int exmodel = (result >> 16) & 0xF;
    printInt(exmodel, 0x0F, true);
    consoleNewLine(3);

    printString("extended family=",0x0D);
    int exfamily = (result >> 20) & 0xFF;
    printInt(exfamily, 0x0F, true);*/
    return true;
}

bool debugCommand1(string input)
{
    /*printString("this is a debug command", 0x0D);
    consoleNewLine(3);
    vga_editPaletteReg(0, blue);*/
    rarray<string> randStrings = rarray<string>();
    for (int i = 0; i < 300; i++)
    {
        randStrings.push_back("poop");
        randStrings.push_back("pee");
        randStrings.push_back("cum");
    }

    consoleNewLine();
    printString("done", 0x0F);
    return true;
}

bool debugCommand2(string input)
{
    /*printString("this is a debug command", 0x0C);
    consoleNewLine(3);
    vga_editPaletteReg(0, black);*/
    rarray<int> randInts = rarray<int>();
    for (int i = 0; i < 300; i++)
    {
        randInts.push_back(69);
        randInts.push_back(420);
        randInts.push_back(9001);
    }
    return true;
}

bool debugCommand3(string input)
{
    /*printString("this is a debug command", 0x0B);
    printInt(getvgamode(), 0x0E, true);
    int x;
    int y;
    resOfVGAMode(&x, &y);
    printInt(x, 0x0F, false);
    printChar('x', 0x0F);
    printInt(y, 0x0F, false);*/
    //commenting this out for string debugging
    
    //print newline for the string shenanigans
    /*consoleNewLine(3);
    printString("creating a string named var1 = '-123';", 0x0F);
    string var1 = "-123";
    breakpointHack();
    printString(var1, 0x0D);
    consoleNewLine(3);
    printString(var1, 0x0D);
    printString(var1.substr(0, 2), 0x0D);
    printString(var1, 0x0D);
    breakpointHack();
    printString("..", 0x0D);
    breakpointHack();
    printString(var1, 0x0E);
    breakpointHack();*/


    /*consoleNewLine();
    unsigned int test = (unsigned int)malloc(100);
    printInt(test, 0x0F);
    consoleNewLine();
    printInt(currentSize, 0x0F);
    consoleNewLine();
    printInt((unsigned int)malloc(100), 0x0F);
    consoleNewLine();
    printInt(currentSize, 0x0F);
    consoleNewLine();
    printInt(getMaxSizeMMT()/sizeof(memData), 0x0F);*/
    unsigned int *data = (unsigned int*)malloc(3000);
    for (int p = 0; p < 3000; p++)
    {
        data[p] = betterRand(p + 3);
    }

    consoleNewLine();
    for (int u = 0; u < 500; u++)
    {
        for (int i = 0; i < 6; i++)
        {
            printInt(data[u+(u*i)], 0x0F, false);
        }
        consoleNewLine();
    }

    //huh, most of the cpu time is spent printing the ints, not calculating the numbers
    free(data);

    return true;
}

//create a memory leak of the specified size. Allocates memory but doesn't free. Potentially useful for debugging
bool memLeak(int size=1)
{
    //alloca(size, 1);
}

bool memoryInfo(string input)
{
    printMemoryManagement();
    return true;
}

bool stringSplitTest(string input)
{
    string testString = "ha ad je";
    rarray<string> dammit = rarray<string>();
    split(testString, ' ', &dammit);    //changing it all to * instead of & didn't make the bug go away
    
    for (int i = 0; i < dammit.getSize(); i++)
    {
        printString(dammit.at(i), 0x0F);
        consoleNewLine(3);
    }

    //delete all the strings to avoid memory leaks. I can't figure out how to make it automatically do that in the rarray deletion functions although in theory it *should* be possible
    for(int i = 0; i < dammit.getSize(); i++)
    {
        dammit.at(i).~string(); //fixed the memory leak created in the aftermath of fixing The C++ Bug
    }
    dammit.clear();

    /*asmOutb('\n', 0xE9);
    intToE9((unsigned int)poop, false);
    asmOutb('\n', 0xE9);

    if (!allocationExists(poop))
    {
        asmOutb('e', 0xE9);
        asmOutb('r', 0xE9);
        asmOutb('r', 0xE9);
        asmOutb('o', 0xE9);
        asmOutb('r', 0xE9);
    }
    asmOutb('y', 0xE9);
    asmOutb('a', 0xE9);
    asmOutb('y', 0xE9);*/

    //printInt(poop.getSize(), 0x0F, false);
    printMemInfo(true);
    return true;
}

bool md5Test(string input)
{
    //cool, c_str works as intended
    //string poop = "poop";
    //printCharArray(poop.c_str(), 0x0F, 160);
    consoleNewLine();
    string yeah = "You entered: ";
    yeah += input;
    printString(yeah, 0x0F);

    MD5 testmd5;
    testmd5.update(input.c_str(), input.length());
    testmd5.finalize();

    consoleNewLine();
    printString("The encrypted hash is: ", 0x0F);
    printString(testmd5.hexdigest(), 0x0F);
    return true;
}

bool aesTest(string input)
{
    consoleNewLine();
    string yeah = "You entered: ";
    yeah += input;
    printString(yeah, 0x0F);
    consoleNewLine();
    /*printString("Converting that to a key gets you: ", 0x0F);
    unsigned char *theKey = (unsigned char*)keyFromString(input);
    for (int i = 0; i < 16; i++)
    {
        print8BitHexInt((char)theKey[i]);
    }
    //free(theKey);//clear the key cause we're done with it
    consoleNewLine();*/
    printString("AES_KEYLEN is set to ", 0x0F);
    printInt(AES_KEYLEN, 0x0F, false);
    consoleNewLine();
    printString("AES_BLOCKLEN is set to ", 0x0F);
    printInt(AES_BLOCKLEN, 0x0F, false);
    consoleNewLine();
    //printString("This command doesn't do anything yet.", 0x0F);

    //do the most basic test first. aes 128
    //char key[] = { (char)0x2b, (char)0x7e, (char)0x15, (char)0x16, (char)0x28, (char)0xae, (char)0xd2, (char)0xa6, (char)0xab, (char)0xf7, (char)0x15, (char)0x88, (char)0x09, (char)0xcf, (char)0x4f, (char)0x3c };
    //char out[] = { (char)0x3a, (char)0xd7, (char)0x7b, (char)0xb4, (char)0x0d, (char)0x7a, (char)0x36, (char)0x60, (char)0xa8, (char)0x9e, (char)0xca, (char)0xf3, (char)0x24, (char)0x66, (char)0xef, (char)0x97 };
    /*char in[]  = { (char)0x6b, (char)0xc1, (char)0xbe, (char)0xe2, (char)0x2e, (char)0x40, (char)0x9f, (char)0x96, (char)0xe9, (char)0x3d, (char)0x7e, (char)0x11, (char)0x73, (char)0x93, (char)0x17, (char)0x2a };

    for (int i = 0; i < 16; i++)
    {
        print8BitHexInt(in[i]);
    }

    struct AES_ctx ctx;
    AES_init_ctx(&ctx, theKey);
    AES_ECB_encrypt(&ctx, in);
    free(theKey);               //done with the encryption key. get rid of it
    consoleNewLine();
    printString("ECB encrypt: ",0x0F);  //ECB is the simplest, followed by CBC and then CTR. CTR is the most secure.
    consoleNewLine();

    for (int i = 0; i < 16; i++)
    {
        print8BitHexInt(in[i]);
    }*/

    printString("numBlocks = ", 0x0F);
    printInt((input.length() / AES_BLOCKLEN), 0x0F);
    consoleNewLine();
    printString("Remainder: ", 0x0F);
    printInt(input.length() % AES_BLOCKLEN, 0x0F);

    unsigned char in[]  = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                      0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                      0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                      0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10 };
    
    unsigned char key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };

    consoleNewLine();
    printString("Encrypted: ", 0x0F);
    string encrypted = encrypt(input, key);
    printString(encrypted, 0x0F);
    consoleNewLine();
    printString("Decrypted: ", 0x0F);
    string decrypted = decrypt(encrypted, key);
    printString(decrypted, 0x0F);

    return true;
}

//aes 128 cbc only
bool basicAesTest(string input)
{
    printString("Welcome to the basic AES test. Well actually, not 'welcome'. Fuck you.",0x0F);
    consoleNewLine();

    //unsigned char key[] = { (unsigned char)0x2b, (unsigned char)0x7e, (unsigned char)0x15, (unsigned char)0x16, (unsigned char)0x28, (unsigned char)0xae, (unsigned char)0xd2, (unsigned char)0xa6, (unsigned char)0xab, (unsigned char)0xf7, (unsigned char)0x15, (unsigned char)0x88, (unsigned char)0x09, (unsigned char)0xcf, (unsigned char)0x4f, (unsigned char)0x3c };
    //unsigned char in[]  = { (unsigned char)0x6b, (unsigned char)0xc1, (unsigned char)0xbe, (unsigned char)0xe2, (unsigned char)0x2e, (unsigned char)0x40, (unsigned char)0x9f, (unsigned char)0x96, (unsigned char)0xe9, (unsigned char)0x3d, (unsigned char)0x7e, (unsigned char)0x11, (unsigned char)0x73, (unsigned char)0x93, (unsigned char)0x17, (unsigned char)0x2a };
    unsigned char iv[]  = { (unsigned char)0x00, (unsigned char)0x01, (unsigned char)0x02, (unsigned char)0x03, (unsigned char)0x04, (unsigned char)0x05, (unsigned char)0x06, (unsigned char)0x07, (unsigned char)0x08, (unsigned char)0x09, (unsigned char)0x0a, (unsigned char)0x0b, (unsigned char)0x0c, (unsigned char)0x0d, (unsigned char)0x0e, (unsigned char)0x0f };

    unsigned char in[]  = { (unsigned char)0x6b, (unsigned char)0xc1, (unsigned char)0xbe, (unsigned char)0xe2, (unsigned char)0x2e, (unsigned char)0x40, (unsigned char)0x9f, (unsigned char)0x96, (unsigned char)0xe9, (unsigned char)0x3d, (unsigned char)0x7e, (unsigned char)0x11, (unsigned char)0x73, (unsigned char)0x93, (unsigned char)0x17, (unsigned char)0x2a,
                      (unsigned char)0xae, (unsigned char)0x2d, (unsigned char)0x8a, (unsigned char)0x57, (unsigned char)0x1e, (unsigned char)0x03, (unsigned char)0xac, (unsigned char)0x9c, (unsigned char)0x9e, (unsigned char)0xb7, (unsigned char)0x6f, (unsigned char)0xac, (unsigned char)0x45, (unsigned char)0xaf, (unsigned char)0x8e, (unsigned char)0x51,
                      (unsigned char)0x30, (unsigned char)0xc8, (unsigned char)0x1c, (unsigned char)0x46, (unsigned char)0xa3, (unsigned char)0x5c, (unsigned char)0xe4, (unsigned char)0x11, (unsigned char)0xe5, (unsigned char)0xfb, (unsigned char)0xc1, (unsigned char)0x19, (unsigned char)0x1a, (unsigned char)0x0a, (unsigned char)0x52, (unsigned char)0xef,
                      (unsigned char)0xf6, (unsigned char)0x9f, (unsigned char)0x24, (unsigned char)0x45, (unsigned char)0xdf, (unsigned char)0x4f, (unsigned char)0x9b, (unsigned char)0x17, (unsigned char)0xad, (unsigned char)0x2b, (unsigned char)0x41, (unsigned char)0x7b, (unsigned char)0xe6, (unsigned char)0x6c, (unsigned char)0x37, (unsigned char)0x10 };

//only the key has to be bigger for aes 265 cbc
    unsigned char key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };

    for (int i = 0; i < 64; i++)
    {
        print8BitHexInt((char)in[i]);
    }
    consoleNewLine();

    struct AES_ctx ctx;
    //AES_ctx_set_iv(&ctx, iv);
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_encrypt_buffer(&ctx, in, 64);
    for (int i = 0; i < 64; i++)
    {
        print8BitHexInt((char)in[i]);
    }
    consoleNewLine();

    //now decrypt it back
    struct AES_ctx ctx2;
    //AES_ctx_set_iv(&ctx, iv);
    char iv2[]  = { (char)0x00, (char)0x01, (char)0x02, (char)0x03, (char)0x04, (char)0x05, (char)0x06, (char)0x07, (char)0x08, (char)0x09, (char)0x0a, (char)0x0b, (char)0x0c, (char)0x0d, (char)0x0e, (char)0x0f };
    AES_init_ctx_iv(&ctx2, key, iv);
    //AES_init_ctx(&ctx2, key);
    AES_CBC_decrypt_buffer(&ctx2, in, 64);
    for (int i = 0; i < 64; i++)
    {
        print8BitHexInt((char)in[i]);
    }

    return true;
}

//aight, I need a basic hex reader now
bool readCommand(string input)
{
    //get the user input address
    consoleNewLine();
    printString("input string length is ", 0x0F);
    printInt(input.length(), 0x0F);
    consoleNewLine();
    unsigned int bullshit = 0;  //does nothing but required for compatibility with gnu programs
    int num = stoi(input, &bullshit, 16);
    printInt(num, 0x0F, false);

    //now that the input address string has been parsed and converted, draw the top rows for the hex reader screen
    consoleNewLine();

    //recently moved to readMemoryHex(int) in utilities.h
    readMemoryHex(num);
    return true;
}

bool atatestCommand(string input)
{
    consoleNewLine();
    if (input.length() < 9)
    {
        printString("Error. You have to enter a number as a parameter.", 0x0F);
        return false;
    }
    int LBA = stoi(input.substr(8, input.length()));        //convert user input to a number
    printString("The LBA address you entered is: ", 0x0F);
    printInt(LBA, 0x0F);
    consoleNewLine();
    //printString("Getting ready to print LBA address 1 from ata device 0..", 0x0D);
    char *array = ata_lba_read(LBA, MBR_DRIVE_MODE, 2);
    readMemoryHex((int)(array), 16);      //32 rows to show 512 bytes
    /*for (int i = 0; i < 512; i++)
    {
        // printChar(array[i], 0x0F);
        char high = intToHexChar((array[i] & 0xF0)>>4);
        char low = intToHexChar(array[i] & 0x0F);
        //printInt(array[i], 0x0F, true);
        printChar(high, 0x0F);
        printChar(low, 0x0F);
        printChar(' ', 0x0F);
    }*/

    // no memory leaks / if the system doesn't crash when this happens, that means its extremely stable
    free(array);

    //printString("..Done", 0x0D);
    //consoleNewLine();

    return true;
}

bool mbrInfoCommand(string input)
{   //mbrinfo 1
    int partitionNumber = 0;
    if (input.length() > 8)
    {
        if(input.at(8) > 47 && input.at(8) < 58)
        {
            partitionNumber = input.at(8) - 48;
        }
    }
    consoleNewLine();
    printString("Selected partition number = ", 0x0F);
    printInt(partitionNumber, 0x0F);
    consoleNewLine();
    //char *partitionEntry = getMbrPartition(partitionNumber, MBR_DRIVE_MODE);

    filesystemInfo what = partitionInformationMBR(MBR_DRIVE_MODE, partitionNumber);
    printString("status byte: 0x", 0x0F);
    print8BitHexInt(what.partitionType);

    /*consoleNewLine();
    printString("head: 0x", 0x0F);
    print8BitHexInt((char)partitionEntry[1]);
    consoleNewLine();
    printString("sector/cyl: 0x", 0x0F);
    print8BitHexInt((char)partitionEntry[2]);
    consoleNewLine();
    printString("cylinder: 0x", 0x0F);
    print8BitHexInt((char)partitionEntry[3]);*/
    consoleNewLine();
    printString("partition type: 0x", 0x0F);
    print8BitHexInt(what.partitionType);

    //do checks of known filesystem types
    if (what.partitionType == 0x06)
    {
        printString("  FAT16, ms-dos bootable", 0x0F);
    }
    else if(what.partitionType == 0x0B)
    {
        printString("  FAT32 with CHS addressing", 0x0F);
    }
    else if(what.partitionType == 0x0C)
    {
        printString("  FAT32 with LBA addressing", 0x0F);
    }

    consoleNewLine();
    printString("Number of sectors in partition: ",0x0F);
    /*print8BitHexInt((char)partitionEntry[0x0F]);
    print8BitHexInt((char)partitionEntry[0x0E]);
    print8BitHexInt((char)partitionEntry[0x0D]);
    print8BitHexInt((char)partitionEntry[0x0C]);*/
    printInt(what.numSectors, 0x0F, true);

    consoleNewLine();
    printString("LBA of first absolute sector in the partition: 0x", 0x0F);
    //unsigned int theNumber = *(unsigned int*)(partitionEntry+8);        //huh, it doesn't work if I do partitionEntry[8] instead. Weird.
    printInt(what.startingLBA, 0x0F, true);

    //char *result = ata_lba_read(what.startingLBA, MBR_DRIVE_MODE);
    
    consoleNewLine();
    //readMemoryHex((int)result, 8);

    //print some information about the partition
    consoleNewLine();
    //short bytesPerSector = *(short*)(result+0x0B);
    //char sectorsPerCluster = *(char*)(result+0x0D);
    //short reservedSectors = *(short*)(result+0x0E);
    printString("Bytes per sector: ", 0x0F);
    printInt(what.bytesPerSector, 0x0F);
    printString("    Sectors per cluster: ", 0x0F);
    printInt(what.sectorsPerCluster, 0x0F);

    //print some more information about the partition
    consoleNewLine();
    printString("Sectors per FAT: ", 0x0F);
    printInt(what.sectorsPerFat, 0x0F);
    printString("    Num reserved sectors: ", 0x0F);
    printInt(what.reservedSectors, 0x0F, false);
    //unsigned int sectorsPerFat = *(unsigned int*)(result+0x24);//was0x24
    //short fsInfoSector = *(short*)(result+0x30);
    //unsigned int serialNumber = *(unsigned int*)(result+0x43);
    
    if (what.partitionType == 0x0B || what.partitionType == 0x0C)
    {
        //this stuff is only valid on fat32
        printString("    FS Info location: ", 0x0F);
        printInt(what.fsInfoSectorLocation, 0x0F), true;

        consoleNewLine();
        printString("Volume serial number: ", 0x0F);
        printInt(what.volumeSerialNumber, 0x0F, true);
    }
    consoleNewLine();
    printString("Number of FATs: ", 0x0F);
    printInt(what.numFats, 0x0F);

    //the same information required to calculate this stuff SHOULD be present in both FAT16 and FAT32
    consoleNewLine();
    unsigned int data = what.startingLBA + (what.reservedSectors) + (what.sectorsPerFat * 2);
    printString("The LBA address of the first file is likely: 0x", 0x0F);
    printInt(data, 0x0F, true);

    //free(partitionEntry);       //no memory leaks. done with mbr partition entry by this point
    //free(result);

    //unsigned int
    //printString("The files should be at: ", 0x0F);
    /*consoleNewLine();
    char *fileData = ata_lba_read(data, MBR_DRIVE_MODE);
    consoleNewLine();
    printAllFiles(data, MBR_DRIVE_MODE, what);
    free(fileData);*/
    /*readMemoryHex((int)fileData,4);*/


    return true;
}

bool mbrInfoSetCommand(string input)
{
    consoleNewLine();
    if (input.at(1) == '1')
    {
        MBR_DRIVE_MODE = 1;
        printString("ata drive set to 1", 0x0F);

    }
    else if (input.at(1) == '0')
    {
        MBR_DRIVE_MODE = 0;
        printString("ata drive set to 0", 0x0F);
    }
    else
    {
        printString("Whatever you typed in was invalid but the selected drive is currently set to: ", 0x0F);
        printInt(MBR_DRIVE_MODE, 0x0F);
    }
    return true;
}

void printAllFiles(unsigned int LBA, unsigned int driveNum, filesystemInfo *sys)
{
    //32 bytes per file I think
    /*char *theSector = ata_lba_read(LBA, driveNum);

    for (int i = 0; i < 16; i++)
    {
        if (printFile((char*)(theSector + (i * 32))))
        {
            consoleNewLine();
        }
    }*/

    printString("getting files in directory", 0x0F);
    rarray<fileInfo> *files = getFilesInDirectory(LBA, driveNum, sys);
    consoleNewLine();
    printString("preparing to print file list..", 0x0F);
    consoleNewLine();
    printFileList(files, sys);

    //we have to erase each string before clearing the rarray because strings contain char arrays
    for (int i = 0; i < files->getSize(); i++)
    {
        files->at(i).~fileInfo();
    }

    //now do all the other auxillary memory leak prevention stuff
    files->manual_delete();
    free(files);

    //there, no memory leaks
}

bool printFile(char *input)
{
    if (input[0] < 34)
    {
        return false;
    }

    for (int i = 0; i < 8; i++)
    {
        if (input[i] > 32)
        {
            printChar(input[i], 0x0F);
        }
    }

    char attrib = input[0x0B];
    if ((attrib & 0x10)==1)
    {
        //don't print the file extension if it's a directory
    }
    else if ((attrib & 0xD2)==0)
    {
        printChar('.', 0x0F);
        printChar(input[8], 0x0F);
        printChar(input[9], 0x0F);
        printChar(input[10], 0x0F);
    }

    //0x14 = high two bytes of cluster location in FAT32
    //0x1A = low two bytes of cluster location in FAT32 and FAT16

    return true;
}

bool lsCommand(string input)
{
    if (selectedVolume.mounted)
    {
        consoleNewLine();
        //print entire directory for the user to see
        //printAllFiles(rootLBA(selectedVolume.fsinfo), MBR_DRIVE_MODE, selectedVolume.fsinfo);
        printAllFiles(selectedVolume.currentDirectoryLBA, MBR_DRIVE_MODE, &selectedVolume.fsinfo);
    }
    else
    {
        printString("Error. Drive not mounted.", 0x0F);
    }
    return true;
}

bool cdCommand(string input)
{
    if (selectedVolume.mounted)
    {
        printString("step1", 0x0F);
        fileInfo foundFile = getFileByName(selectedVolume.currentDirectoryLBA, MBR_DRIVE_MODE, &selectedVolume.fsinfo, input);
        printString("step2", 0x0F);
        if (foundFile.isValidFile && foundFile.isDirectory)
        {
            printString("Found file has an LBA address of ", 0x0F);
            selectedVolume.currentDirectoryLBA = LBAofFile(&foundFile, &selectedVolume.fsinfo);
            printInt(selectedVolume.currentDirectoryLBA, 0x0F, true);   //update the parent directory LBA pointer
        }
        else if (foundFile.isValidFile)
        {
            //prevent the user from messing up the entire system. cd into files is disallowed
            printString("You can't cd into a file, it has to be a directory.",0x0F);
        }
        else
        {
            printString("Directory named '", 0x0F);
            printString(input, 0x0F);
            printString("' not found", 0x0F);
        }
        return true;
    }
    else
    {
        printString("Error. Drive not mounted.", 0x0F);
        return true;
    }
    return true;
}

bool mountCommand(unsigned int partitionNumber)
{
    //information about the partition we want to mount
    //dataVolumeInfo dsInfo;
    
    //first, generate a filesystem info
    selectedVolume.fsinfo = partitionInformationMBR(MBR_DRIVE_MODE, partitionNumber);

    consoleNewLine();
    printString("partition number ", 0x0F);
    printInt(partitionNumber, 0x0F);
    printString(" on drive #", 0x0F);
    printInt(MBR_DRIVE_MODE, 0x0F);
    consoleNewLine();

    selectedVolume.mounted = true;
    selectedVolume.currentDirectoryLBA = rootLBA(&selectedVolume.fsinfo);        //since this is a newly mounted volume, reset the folder location LBA to the root directory

    return true;
}

bool fileSizeCommand(string input)
{
    consoleNewLine();       //get this part out of the way since I have to do it sooner or later

    fileInfo foundFile = getFileByName(selectedVolume.currentDirectoryLBA, MBR_DRIVE_MODE, &selectedVolume.fsinfo, input);
    if (foundFile.isValidFile)
    {
        //directories don't have valid data in their filesize offset
        if (!foundFile.isDirectory)
        {
            printString("Size: ", 0x0F);
            printInt(foundFile.size, 0x0F);
        }
        printString(". Size on filesystem: ",0x0F);
        printInt(sizeOfFile(&foundFile, &selectedVolume.fsinfo, MBR_DRIVE_MODE), 0x0F);

        /*consoleNewLine();
        printString("fats: ", 0x0F);
        rarray<unsigned int> *blocks = fileClusters(foundFile, selectedVolume.fsinfo, MBR_DRIVE_MODE);
        for (int i = 0; i < blocks->getSize(); i++)
        {
            consoleNewLine();
            printInt(blocks->at(i), 0x0F, true);
        }
        consoleNewLine();
        printInt(blocks->getSize(), 0x0F);
        printString(" blocks.", 0x0F);
        //now do all the other auxillary memory leak prevention stuff
        blocks->manual_delete();
        //free(blocks);
        */



    }
    else
    {
        printString("Not found", 0x0F);
    }
    foundFile.~fileInfo();//delete the file dynamic memory

    return true;
}

bool fatLoadCommand(string input)
{
    //printString(input, 0x0E);
    consoleNewLine();
    fileInfo foundFile = getFileByName(selectedVolume.currentDirectoryLBA, MBR_DRIVE_MODE, &selectedVolume.fsinfo, input);
    if (foundFile.isValidFile)
    {
        /*//get a list of clusters that the file takes up
        rarray<unsigned int> *blocks = fileClusters(&foundFile, &selectedVolume.fsinfo, MBR_DRIVE_MODE);

        //allocate a spot in memory to store the file
        char *fileStore = (char *)malloc(sizeOfFile(&foundFile, &selectedVolume.fsinfo, MBR_DRIVE_MODE));
        unsigned int debugInt = 0;
        for (int i = 0; i < blocks->getSize(); i++)
        {
            char poop[3];
            *(unsigned int*)poop = blocks->at(i);
            if (!isExitCodeFAT(poop, &selectedVolume.fsinfo))
            {
                for (int s = 0; s < selectedVolume.fsinfo.sectorsPerCluster; s++)
                {
                    unsigned int lbaOfFileCluster = ((blocks->at(i)-2) * selectedVolume.fsinfo.sectorsPerCluster) + rootLBA(&selectedVolume.fsinfo) + s;
                    //lbaOfFileCluster-=2;     //you need to do this for some reason. I'm sure FAT16 will need other special handling but I already had to give up on FAT16 in fileClusters
                    //*(char*)(fileStore+(i*512)) = ata_lba_read(lbaOfFileCluster, MBR_DRIVE_MODE);
                    char *bytes = ata_lba_read(lbaOfFileCluster, MBR_DRIVE_MODE);
                    memcpy((fileStore+(((i*selectedVolume.fsinfo.sectorsPerCluster)+s) * 512)), (bytes), 512);        //memcpy is the most optimized memory copy right now
                    debugInt++;
                    free(bytes); // don't need this anymore
                }
            }
        }
        consoleNewLine();
        printInt(debugInt, 0x0F);
        consoleNewLine();

        //print the raw contents to the screen
        //consoleNewLine();
        unsigned int count = 0;
        while (count < foundFile.size)
        {
            readMemoryHex((unsigned int)fileStore + count);
            consoleNewLine();
            //printString("Press any key to view the next 256 bytes. You are on byte ",0x0F);
            printInt(count, 0x0F);
            consoleNewLine();
            //waitChar();
            count += 256;
        }

        //before any freeing happens, display how much memory is in use
        printMemoryManagement();

        free(fileStore);    //avoid memory leak
        //if forgetting to free this doesn't cause major memory leaks then something is really wrong
        
        blocks->manual_delete();
        free(blocks);*/
        void *storedFile = loadFile(&foundFile, MBR_DRIVE_MODE, &selectedVolume.fsinfo);

        consoleNewLine();

        //print the raw contents to the screen
        //consoleNewLine();
        unsigned int count = 0;
        while (count < foundFile.size)
        {
            readMemoryHex((unsigned int)storedFile + count);
            consoleNewLine();
            //printString("Press any key to view the next 256 bytes. You are on byte ",0x0F);
            printInt(count, 0x0F);
            consoleNewLine();
            //waitChar();
            count += 256;
        }

        printMemoryManagement();
        free(storedFile);    //avoid memory leak
    }
    else
    {
        printString("Not found", 0x0F);
    }

    foundFile.~fileInfo();//delete the file dynamic memory

    return true;
}

//boot a binary file in protected mode. Most binary files need to be booted in real mode.
//the only purpose of this function is to boot a c++ os binary, since the bootloader and system have to be seperate
bool boot32(string input)
{
    printString("You can only do that from the bootloader.", 0x0F);
    return true;
}

//view a file as text.
bool viewCommand(string input)
{
    //load file to ram
    consoleNewLine();
    printString("Loading file: ", 0x0F);
    printString(input, 0x0F);
    fileInfo systemFile = getFileByName(selectedVolume.currentDirectoryLBA, MBR_DRIVE_MODE, &selectedVolume.fsinfo, input);
    consoleNewLine();
    printString("File found. Loading...", 0x0F);
    void* systemData = loadFile(&systemFile, MBR_DRIVE_MODE, &selectedVolume.fsinfo);
    consoleNewLine();
    printString("File loaded. Here are the contents:",0x0F);
    consoleNewLine();
    for (int i = 0; i < systemFile.size; i++)
    {
        printChar(*(char*)(systemData+i), 0x0F);
        //big brain time aka stupid lazy hack time
        checkIfNewlineNeeded();
    }

    free(systemData);       //only YOU can prevent memory leaks

    return true;
}

//view a file as a 320x200 8 bit color image
bool gViewCommand(string input)
{
    //load file to ram
    consoleNewLine();
    printString("Loading file: ", 0x0F);
    printString(input, 0x0F);
    fileInfo systemFile = getFileByName(selectedVolume.currentDirectoryLBA, MBR_DRIVE_MODE, &selectedVolume.fsinfo, input);
    consoleNewLine();
    if (systemFile.isValidFile)
    {
        printString("File found. Loading...", 0x0F);
        void *systemData = loadFile(&systemFile, MBR_DRIVE_MODE, &selectedVolume.fsinfo);
        consoleNewLine();
        printString("File loaded. Attempting to display as a picture..", 0x0F);

        vga300200ShittyAssBullshit256.applyToSystem();
        for (int i = 0; i < 60000; i++)
        {
            *(char *)(0xA0000 + (i)) = 0;
        }
        drawImage(systemData);

        //================
        //its loaded, now browse to the palette file
        cdCommand("..");
        cdCommand("SYSTEM");
        cdCommand("PALETTES");
        ploadCommand("DEFAULT");
        //damn, loading the palettes AFTER switching to the graphics mode doesn't fix it. shit.
        //the color palette is still incorrect in the same exact way though so at least I don't have to keep doing it this way

        waitChar();
        free(systemData);
        systemFile.~fileInfo();

        vgaDefaultTextmode.applyToSystem();
    }
    else
    {
        printString("File not found", 0x0F);
    }

    return true;
}

//loads a 256 color palette from a file
//as of 05/27/2024, this runs at a borderline acceptable speed on a 160mhz 5x86. Parsing the text file data is slow
bool ploadCommand(string input)
{
    //load file to ram
    consoleNewLine();
    printString("Loading file: ", 0x0F);
    printString(input, 0x0F);
    fileInfo systemFile = getFileByName(selectedVolume.currentDirectoryLBA, MBR_DRIVE_MODE, &selectedVolume.fsinfo, input);
    consoleNewLine();

    if (systemFile.isValidFile)
    {
        printString("File found. Loading...", 0x0F);
        void *systemData = loadFile(&systemFile, MBR_DRIVE_MODE, &selectedVolume.fsinfo);
        consoleNewLine();
        printString("File loaded. Attempting to process color values..", 0x0F);
        //rarray<string> lines = rarray<string>();
        rarray<short> nums = rarray<short>();
        //lines.push_back("poop");
        //lines.push_back("pee");
        //getLinesOfPaletteFile(systemData, systemFile.size, &lines);
        getValuesOfPaletteFile(systemData, systemFile.size, &nums);
        printString("Data has been processed. Printing..", 0x0F);
        asmOutb(0, PEL_ADDRESS_WRITE);
        for (int i = 6; i < nums.getSize(); i+=3)
        //for (int i = 0; i < 36; i+=3)
        {
            //printString(lines.at(i), 0x0F);
            unsigned int index = (i-6)/3;
            /*printInt(index, 0x0F);
            printChar(' ', 0x0F);
            printChar(' ', 0x0F);
            printChar(' ', 0x0F);
            printInt(0x000000FF&nums.at(i), 0x0F);
            printChar(' ', 0x0F);
            printInt(0x000000FF&nums.at(i+1), 0x0F);
            printChar(' ', 0x0F);
            printInt(0x000000FF&nums.at(i+2), 0x0F);
            consoleNewLine();*/
            //PEL_write(index, nums.at(i), nums.at(i+1), nums.at(i+2));
            PEL_write_autoincrement(nums.at(i), nums.at(i+1), nums.at(i+2));
            //PEL_write(2, 0xFF, 0xFF, 0xFF);
            //PEL_write(3, 0x00, 0x00, 0x00);
        }

        /*unsigned int longestStringSoFar = 0;
        for (int i = 0; i < nums.getSize(); i++)
        {
            if (nums.at(i).length() > longestStringSoFar)
            {
                longestStringSoFar = nums.at(i).length();
            }
        }
        printString("The longest string length is ", 0x0F);
        printInt(longestStringSoFar, 0x0F, false);
        consoleNewLine();*/

        unsigned int longestMemSoFar = 0;
        unsigned int location = 0;
        for (int i = 0; i < currentSize; i++)
        {
            memData *ptr = &mmt[i];
            unsigned int val = ptr[i].m_end - ptr[i].m_start;
            if (val > longestMemSoFar)
            {
                longestMemSoFar = val;
                location = i;
            }
        }
        printString("The longest memory allocation length is ", 0x0F);
        printInt(longestMemSoFar, 0x0F, false);
        printString(" at location ", 0x0F);
        printInt(location, 0x0F);
        consoleNewLine();
        printString("Mem size = ", 0x0F);
        printInt(currentSize, 0x0F);
        consoleNewLine();

        /*or(int i = 0; i < lines.getSize(); i++)
        {
            lines.at(i).~string();
        }*/
        nums.manual_delete();
        free(systemData);
    }
    else
    {
        printString("File not found", 0x0F);
    }

    return true;
}

bool vgaInfoCommand(string input)
{
    char settings = 0x00;
    if (input.contains("-"))
    {
        if (input.contains("s"))
        {
            settings = settings | 0x01;
        }
        if (input.contains("g"))
        {
            settings = settings | 0x02;
        }
        if (input.contains("c"))
        {
            settings = settings | 0x04;
        }
        if (input.contains("a"))
        {
            settings = settings | 0x08;
        }
        if (input.contains("m"))
        {
            settings = settings | 0x10;
        }
    }

    if (input.contains("-h") || input.contains("-help"))
    {
        printString("Options:",0x0F);
        consoleNewLine();
        printString("-s     sequencer registers",0x0F);
        consoleNewLine();
        printString("-g     graphics control registers",0x0F);
        consoleNewLine();
        printString("-c     crtc registers",0x0F);
        consoleNewLine();
        printString("-a     attribute registers",0x0F);
        consoleNewLine();
        printString("-m     the miscellaneous register",0x0F);
        consoleNewLine();
        printString("Note that the displayed registers are not a comprehensive list and are only the ones required for setting new video modes.",0x0F);
    }
    else
    {
        printVgaModeInfo(settings);
    }
    //consoleNewLine();
    //printString("a random vga port = ", 0x0F);
    //asmOutb(0x14, 0x3C1);
    //char result = asmInb(0x3C1);
    //print8BitHexInt(result);
    //attributeRegisterWrite(0x10, 0x01);
    //print8BitHexInt(attributeRegisterRead(0x10));
    //consoleNewLine();
    return true;
}

bool vgaDumpCommand(string input)
{
    char settings = 0x00;
    
    if (input.contains("-"))
    {
        if (input.contains("s"))
        {
            settings = settings | 0x01;
        }
        if (input.contains("g"))
        {
            settings = settings | 0x02;
        }
        if (input.contains("c"))
        {
            settings = settings | 0x04;
        }
        if (input.contains("a"))
        {
            settings = settings | 0x08;
        }
        if (input.contains("m"))
        {
            settings = settings | 0x10;
        }
    }

    if (input.contains("-h") || input.contains("-help"))
    {
        //show the vga help command since the options are the same for this command as the other one
        vgaInfoCommand("--help");
    }
    else
    {
        vga_videoMode currentMode;
        currentMode.dumpCurrentVideoMode();
        currentMode.printRegisters(settings);
        currentMode.manual_delete();
    }

    return true;
}

bool gTestCommand(string input)
{

    /*printString("switching to graphics mode...", 0x0F);
    mode13hSetup();
    printString("switching to graphics mode...", 0x0F);
    //asmOutb(0x00, 0x3CD);*/
    vga300200ShittyAssBullshit256.applyToSystem();
    for (int i = 0; i < 65000; i++)
    {
        *(char*)(0xA0000+(i)) = 0;
    }
    drawTestPattern256();
    bhm_line(100, 200, 50, 75, 3);

    unsigned int x = 0;
    unsigned int y = 0;
    for (int c = 32; c < 143; c++)
    {
        graphicsModeDrawText(x*8, y, c, 0x02, 0xFE, false);
        x++;
        if (x*8 > 319)
        {
            x = 0;
            y+=8;
        }
    }

    //graphicsModeDrawText(0, 0, 'A', 0x02, 0xFE, false);

    //vgaDefaultTextmode.manual_delete();
    waitChar();
    vgaDefaultTextmode.applyToSystem();
    //vga_videoMode thisMode;
    //thisMode.dumpCurrentVideoMode();
    //thisMode.applyToSystem();
    
    /*if (vga_testcrtc())
    {
        printString("crtc test success", 0x0F);
    }
    else
    {
        printString("crtc test failed", 0x0F);
    }

    if (vga_testSequencer())
    {
        consoleNewLine();
        printString("sequencer test success", 0x0F);
    }
    else
    {
        consoleNewLine();
        printString("sequencer test failed", 0x0F);
    }

    if (vga_testGraphicsControl())
    {
        consoleNewLine();
        printString("graphics control test success", 0x0F);
    }
    else
    {
        consoleNewLine();
        printString("graphics control test failed", 0x0F);
    }*/

    /*printString("Current value of sequencer register index 0x00: ", 0x0F);
    printInt(sequencerRegisterRead(0x01), 0x0F, true);
    consoleNewLine();

    char originalValue = sequencerRegisterRead(0x01);
    sequencerRegisterWrite(0x01, 0x66);
    char result = sequencerRegisterRead(0x01);
    sequencerRegisterWrite(0x01, originalValue);

    printString("after attempting to write, its: ", 0x0F);
    printInt(result, 0x0F, true);
    consoleNewLine();

    printString("right now, its: ", 0x0F);
    printInt(sequencerRegisterRead(0x01), 0x0F, true);*/

    return true;
}

//runs some test stuff for graphics mode fonts
bool fontInfo(string input)
{
    for (int i = 0; i < 8; i++)
    {
        unsigned char b = *(unsigned char*)(vgafont8+i+8);
        printInt(0x000000FF&b, 0x0F, true);
        consoleNewLine();
    }
    return true;
}

//searches all of low ram for a string
bool findVesaCommand(string input)
{
    bool foundYet = false;
    unsigned int searchAddress = 0;
    for (int i = 0; i < 0xFFFF0; i++)
    {
        if (*(char*)(searchAddress) == 'V' && *(char*)(searchAddress+1) == 'E' && *(char*)(searchAddress+2) == 'S' && *(char*)(searchAddress+3) == 'A')
        {
            consoleNewLine();
            printString("found at location ", 0x0F);
            printInt(searchAddress, 0x0F, true);
            foundYet = true;
        }
        searchAddress++;
    }

    if (!foundYet)
    {
        //the word "VESA" has to be banned from the source code to avoid false signature detections
        printString("No signature found anywhere in low ram.", 0x0F);
    }
    return true;
}

bool doFpgaVideoCardTest(string input)
{

    return true;
}

bool inCommand(string input)
{
    string newInput = input;
    newInput += " ";
    rarray<string> args;
    split(newInput, ' ', &args);

    for (int i = 0; i < args.getSize(); i++)
    {
        consoleNewLine();
        printString(args.at(i), 0x0D);
    }

    if (args.getSize() < 1)
    {
        consoleNewLine();
        printString("Incorrect number of args. Expected 1 space-delimited argument. Example: 'in 426", 0x0F);
    }
    else
    {
        unsigned int bullshit = 0;
        unsigned short address = stoi(args.at(1), &bullshit, 16);//everything is in base 16

        consoleNewLine();
        printString("reading port ", 0x0F);
        printInt(address, 0x0E, true);
        unsigned char c = inb(address);
        consoleNewLine();
        printString("result = ", 0x0F);
        printInt(c, 0x0E, true);
    }
    return true;
}

bool outCommand(string input)
{
    string newInput = input;
    newInput += " ";
    rarray<string> args;
    split(newInput, ' ', &args);

    if (args.getSize() < 2)
    {
        consoleNewLine();
        printString("Incorrect number of args. Expected 2 space-delimited args. Example: 'out 420 FF", 0x0F);
    }
    else
    {
        unsigned int bullshit = 0;
        unsigned short address = stoi(args.at(1), &bullshit, 16);//everything is in base 16
        unsigned char data = stoi(args.at(2), &bullshit, 16);

        consoleNewLine();
        printString("writing ", 0x0F);
        printInt(data, 0x0E, true);
        printString(" to port ", 0x0F);
        printInt(address, 0x0E, true);
        outb(data, address);
    }
    return true;
}

bool writeCommand(string input)
{
    //get the user input address
    /*consoleNewLine();
    printString("input string length is ", 0x0F);
    printInt(input.length(), 0x0F);
    consoleNewLine();
    unsigned int bullshit = 0;  //does nothing but required for compatibility with gnu programs
    int num = stoi(input, &bullshit, 16);
    printInt(num, 0x0F, false);


    //now that the input address string has been parsed and converted, draw the top rows for the hex reader screen
    consoleNewLine();

    //recently moved to readMemoryHex(int) in utilities.h
    readMemoryHex(num);*/

    string newInput = input;
    newInput += " ";
    rarray<string> args;
    split(newInput, ' ', &args);

    if (args.getSize() < 2)
    {
        consoleNewLine();
        printString("Incorrect number of args. Expected 2 space-delimited args. Example: 'write A0000 FF", 0x0F);
    }
    else
    {
        unsigned int bullshit = 0;
        unsigned int address = stoi(args.at(1), &bullshit, 16);//everything is in base 16
        unsigned char data = stoi(args.at(2), &bullshit, 16);

        consoleNewLine();
        printString("writing ", 0x0F);
        printInt(data, 0x0E, true);
        printString(" to memory address ", 0x0F);
        printInt(address, 0x0E, true);
        *(char*)(address) = data;
    }

    return true;
}

bool haltCommand(string input)
{
    __asm__("hlt");
    return true;
}

void fpgavideotestoption3helper(unsigned int length, unsigned short color, unsigned int timeoutAmount, unsigned int delayAmount)
{
    outb(0x00, 0x428);
    outb(0x00, 0x428);
    outb(0x00, 0x429);
    outb(0x00, 0x429);
    outb(0x00, 0x42A);
    outb(0x00, 0x42A);

    // write all the bullshit
    unsigned int timeoutCount = 0;
    unsigned int fullCount = 0;
    for (int i = 0; i < length; i++)
    {
        unsigned char status = inb(0x426);
        unsigned char full = status & 0x30;
        int timeout = 0;
        while (timeout < timeoutAmount && (full != 0x00))
        {
            timeout++;
            status = inb(0x426);
            full = status & 0x30;

            if (delayAmount > 2)
            {
                for (int d = 0; d < delayAmount; d++)
                {
                    int wastetime = 4;
                    wastetime = wastetime + 1;
                }
            }
        }

        if (timeout >= timeoutAmount)
        {
            timeoutCount++;

            if (full != 0x00)
            {
                fullCount++;
            }
        }

        outw(color, 0x42C);
    }

    printString("wrote a bunch of bytes to port 0x42C starting at vram address 0x00", 0x0F);
    consoleNewLine();
    printString("timeoutCount = ", 0x0F);
    printInt(timeoutCount, 0x0F, false);
    consoleNewLine();
    printString("fullCount = ", 0x0F);
    printInt(fullCount, 0x0F, false);
}