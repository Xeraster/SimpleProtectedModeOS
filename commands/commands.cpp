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
            printString("Yeeeeaah no. Go to: https://github.com/Xeraster/SimpleProtectedModeOS and figure it out yourself noob", 0x0F);
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
        consoleNewLine(3);
        usedMemory(input);
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

    else if (input == "memleak")
    {
        memLeak(1);
        return true;
    }

    else if (input == "meminfo")
    {
        consoleNewLine(3);
        memoryInfo(input);
        consoleNewLine(3);
        return true;
    }

    //md5 command. need help figuring out which version? the md5 command was added 10/18/2018
    else if (input == "md5")
    {
        consoleNewLine(3);
        printString("Sike! Not implemented lol get wrecked", 0x3F);
        return true;
    }

    else if (input == "atatest")
    {
        printString("Getting ready to print LBA address 1 from ata device 0..", 0x0D);
        consoleNewLine(3);
        char *array = ata_lba_read(0x00);
        for (int i = 0; i < 512; i++)
        {
            //printChar(array[i], 0x0F);
            printInt(array[i], 0x0F, true);
            printChar(' ', 0x0F);
        }

        //no memory leaks / if the system doesn't crash when this happens, that means its extremely stable
        free(array);

        printString("..Done", 0x0D);
        return true;
    }

    //else if (temporary.substr(0, 4) == "split")
    else if (temporary == "split")
    {
        stringSplitTest("todo");
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
    printString("this is a debug command", 0x0D);
    consoleNewLine(3);
    vga_editPaletteReg(0, blue);
    return true;
}

bool debugCommand2(string input)
{
    printString("this is a debug command", 0x0C);
    consoleNewLine(3);
    vga_editPaletteReg(0, black);
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
    consoleNewLine(3);
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
    breakpointHack();
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
    consoleNewLine(3);
    return true;
}

bool stringSplitTest(string input)
{
    string testString = "ha ad je";
    rarray<string> dammit = rarray<string>();
    split(testString, ' ', &dammit);

    for (int i = 0; i < dammit.getSize(); i++)
    {
        printString(dammit.at(i), 0x0F);
        consoleNewLine(3);
    }

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