bool parseCommand(string input)
{
    
    //help command
    //string helpQuery = input.substr(0,5);
    string temporary = input;
    if (temporary.substr(0,5) == "help ")
    //if (temporary.at(0) == 'h' && temporary.at(1) == 'e' && temporary.at(2) == 'l' && temporary.at(3) == 'p')
    {
        if (temporary.contains("-p"))
        {
            consoleNewLine(3);
            printString("Yeeeeaah no. Go to: https://github.com/Xeraster/SimpleProtectedModeOS and figure out it out yourself noob", 0x0F);
            return true;
        }
        else if (temporary.contains("-f"))
        {
            consoleNewLine(3);
            printString("Nice try, but the -f switch only does this", 0x0F);
            consoleNewLine(3);
            printString("maybe try saying please?", 0x0F);
            return true;
        }
        else if (temporary.contains("-h"))
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

    }

    else if (temporary.substr(0,8) == "pcicheck")
    {
        consoleNewLine(3);
        pciCheck(temporary);
        return true;
    }

    else if (temporary == "mem")
    {
        consoleNewLine(3);
        usedMemory(input);
        return true;
    }

    //printString(temporary, 0x0E);
    printString(temporary, 0x0D);

    //printString(input, 0x0E);
    //print32bitNumber(input, 1800);
    return false;
}

bool usedMemory(string input)
{
    //get amount of used dynamic memory
    unsigned int usedMemory = 0;
	for (int i = 0; i < memf.getSize(); i++)
	{
		//usedMemory += getSizeOfMemBlock((void*)memf.at(i), nullptr);
        usedMemory += getSizeOfMemBlock((void*)memf.at(i), 0);
	}

    printInt(usedMemory, 0x0F);
    printString(" bytes in use",0x0F);
    return true;
}

bool pciCheck(string input)
{
    //subtract the ascii value by 48. This means as long as the user only enters 0-9 in position index 10, it will work fine
    char checkSlot = (input[10])-48;
    char device = 0;

    //I am hardcoding the pci bus to zero because on qemu and the ms4144 socket 3 motherboard I have, bios interrupt 1A, BX=0101 reports "last pci bus index" to be zero
    unsigned int vendor = pciCheckVendor(0, checkSlot, &device);

    //now print that garbage to the screen
    printString("Vendor: 0x", 0x0F);
    printInt(vendor, 0x0F, true);

    consoleNewLine(3);
    printString("Device: 0x", 0x0F);
    printInt(device, 0x0F, true);

    return true;

}