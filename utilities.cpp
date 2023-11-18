//figure out how much memory the system has
void memoryTest(bool printToScreen, unsigned int startAddress, unsigned int kbToTest)
{
	bool success = true;
	char* currentAddress = (char*)startAddress;
	//print32bitNumber(startAddress, 100);
	while (success && ((unsigned int)currentAddress - (unsigned int)startAddress)/1000 < kbToTest)
	{
		//first, set the address to zero then check if it's zero when read
		*currentAddress = 0x00;
		char read = *(char*)currentAddress;
		if (read != 0x00)
		{
			success = false;
		}

		//next, set the address to 255 then check to see if it changed to 255
		*currentAddress = 0xFF;
		read = *(char*)currentAddress;
		if (read != 0xFF)
		{
			success = false;
		}

		//set the memory location back to zero before going on to the next one
		*currentAddress = 0x00;

		currentAddress++;

		if (printToScreen)
		{
			printCharAdr('m', 0x0F, 95);
			printCharAdr('e', 0x0F, 96);
			printCharAdr('m', 0x0F, 97);
			printCharAdr(':', 0x0F, 98);
			print32bitNumber((unsigned int)currentAddress - (unsigned int)startAddress, 100);
			printCharAdr('b', 0x0F, 111);
			printCharAdr('y', 0x0F, 112);
			printCharAdr('t', 0x0F, 113);
			printCharAdr('e', 0x0F, 114);
			printCharAdr('s', 0x0F, 115);
		}
	}

	//return (unsigned int)currentAddress - (unsigned int)startAddress;
	return;
}

//takes an 8 bit value and prints it to the screen
void print8bitNumber(unsigned int num, unsigned int curPos)
{
    char printableResult[3];

	printableResult[0] = (num % 10) + 48;		//least significant digit
	printableResult[1] = ((num / 10) % 10) + 48; //middle digit
    printableResult[2] = ((num / 100)) + 48;  	//most significant digit
	printCharAdr(printableResult[0], 0x0F, curPos + 2);
	printCharAdr(printableResult[1], 0x0F, curPos + 1);
    printCharAdr(printableResult[2], 0x0F, curPos);
    
    return;

}

//takes a 16 bit value and prints it to the screen
void print16bitNumber(unsigned int num, unsigned int curPos)
{

	//yes, there are more "better" ways to do this but at the time of this writing, I have no memory management system in place for dynamically declaring array sizes on the fly. These functions are to allow me to debug stuff until I get a better system working
	char printableResult[5]; //designate 5 spaces for the maximum sized 16 bit integer

	printableResult[0] = (num % 10) + 48;		//least significant digit
	printableResult[1] = ((num / 10) % 10) + 48;
    printableResult[2] = ((num / 100) % 10) + 48;
	printableResult[3] = ((num / 1000) % 10) + 48;
	printableResult[4] = ((num / 10000) % 10) + 48;  	//most significant digit

	printCharAdr(printableResult[0], 0x0F, curPos + 4);
	printCharAdr(printableResult[1], 0x0F, curPos + 3);
    printCharAdr(printableResult[2], 0x0F, curPos + 2);
	printCharAdr(printableResult[3], 0x0F, curPos + 1);
	printCharAdr(printableResult[4], 0x0F, curPos);

	return;

}

//takes an 32 bit value and prints it to the screen
void print32bitNumber(unsigned int num, unsigned int curPos)
{
	char printableResult[10]; //designate 5 spaces for the maximum sized 16 bit integer

	printableResult[0] = (num % 10) + 48;		//least significant digit
	printableResult[1] = ((num / 10) % 10) + 48;
    printableResult[2] = ((num / 100) % 10) + 48;
	printableResult[3] = ((num / 1000) % 10) + 48;
	printableResult[4] = ((num / 10000) % 10) + 48; 
	printableResult[5] = ((num / 100000) % 10) + 48; 
	printableResult[6] = ((num / 1000000) % 10) + 48; 
	printableResult[7] = ((num / 10000000) % 10) + 48; 
	printableResult[8] = ((num / 100000000) % 10) + 48; 
	printableResult[9] = ((num / 1000000000) % 10) + 48; //most significant digit

	printCharAdr(printableResult[0], 0x0F, curPos + 9);
	printCharAdr(printableResult[1], 0x0F, curPos + 8);
    printCharAdr(printableResult[2], 0x0F, curPos + 7);
	printCharAdr(printableResult[3], 0x0F, curPos + 6);
	printCharAdr(printableResult[4], 0x0F, curPos + 5);
	printCharAdr(printableResult[5], 0x0F, curPos + 4);
	printCharAdr(printableResult[6], 0x0F, curPos + 3);
	printCharAdr(printableResult[7], 0x0F, curPos + 2);
	printCharAdr(printableResult[8], 0x0F, curPos + 1);
	printCharAdr(printableResult[9], 0x0F, curPos);
}

//slower than the other number printing functions but more versatile and it doesn't have leading zeros
void printAnyNumber(unsigned int num, short color, unsigned int curPos)
{
	unsigned int numLength = 0;
	unsigned int result = (num % 10) + 48;
	int i = 1;
	while (i < 10)
	{
		result = ((num / (10)) % 10) + 48;
		printCharAdr(char(result), color, curPos + i);
		i++;
	}
	numLength = i;
	//now that we know the length, do the same thing except print the number and don't do any leading zeros
	while (i > 0)
	{
		i--;
		unsigned int thingToPrint = ((num / i) % 10) + 48;
		char output = thingToPrint;
		//printCharAdr(output, color, curPos + i);
	}

	return;
}

//adds 2 numbers together then returns the result
int testIfReturnTypesWork(int num1, int num2)
{
	return num1 + num2;
}

void setCharEqual(char *target, unsigned int *poop)
{
	print32bitNumber(poop[0], 81);
	return;
}

//prints the memory address of all occurances of the word "fooo" in the given memory range 
void findFooo(unsigned int memStart, unsigned int length, int curPos)
{
	unsigned int foosFound = 0;
	for (int i = memStart; i < memStart + length && foosFound < 10; i++)
	{
		if (*(char*)(i) == 'f' && *(char*)(i+1) == 'o' && *(char*)(i+2) == 'o' && *(char*)(i+3) == 'o')
		{
			print32bitNumber(i, 150+(foosFound*80));
			foosFound++;
		}
	}

	print32bitNumber(69, 70);

	return;
}

//shifts the cursor to the left or right. Positive values shift right. Negative values shift left.
void shiftCursor(int howFar)
{
	videoStart+=(howFar*2);
	setVGAtextModeCursor(3);

	return;
}

//performs the equivalent of backspace on textmode 7 or 3 or whatever the default one is
void backspaceScreen()
{
	//shift cursor back 1
	shiftCursor(-1);

	//replace this spot with a blank space
	printChar(' ', 0x0F);

	//go back one to simulate the back spacing type effect
	shiftCursor(-1);

	//now shift everything to the right of the cursor position and on this line only to the left once
	unsigned int X = (((unsigned int)videoStart-0xB8000)/2) % 80;
	unsigned int Y = (((unsigned int)videoStart-0xB8000)/160);

	char *rowAdr = (char*)(0xB8000 + ((Y * 80))*2) + 1;
	for(int i = (X*2); i < 160; i++)
	{
		*(rowAdr + i) = *(char*)(rowAdr + i + 2);
	}



	return;
}

/*void printMemoryAllocation(bool contents)
{
	unsigned int X = (((unsigned int)videoStart-0xB8000)/2) % 80;
	unsigned int Y = (((unsigned int)videoStart-0xB8000)/160);

	unsigned int usedMemory = 0;
	for (int i = 0; i < memf.getSize(); i++)
	{
		//some cross compilers dont do nullptr
		usedMemory += getSizeOfMemBlock((void*)memf.at(i), nullptr);
		//usedMemory += getSizeOfMemBlock((void*)memf.at(i), 0);
	}

	print32bitNumber(usedMemory, X+(Y*80));
	printCharArray(" bytes", 0x0F, X+(Y*80)+10);

	Y++;

	//first, completely erase anything that would be behind this
	for (int i = 0; i < memf.getSize() + 6; i++)
	{
		//print16bitNumber(getSizeOfMemBlock((void*)memf.at(i), nullptr), X-6+((i+Y)*80));
		printCharArray("                           ", 0x0E, X-6+((i+Y)*80));
	}

	for (int i = 0; i < memf.getSize(); i++)
	{
		//print16bitNumber(getSizeOfMemBlock((void*)memf.at(i), nullptr), X-6+((i+Y)*80));
		print16bitNumber(getSizeOfMemBlock((void*)memf.at(i), nullptr), X-6+((i+Y)*80));
		print32bitNumber((unsigned int)memf.at(i), X+((i+Y)*80));
		print32bitNumber((unsigned int)memb.at(i), X+11+((i+Y)*80));
		printCharAdr('-',0x0F, X+10+((i+Y)*80));
		//print contents if applicable
		if (contents)
		{
			char v = ' ';
			for (int c = 0; c < 10 && v != char(0); c++)
			{
				v = *(char*)(memf.at(i)+c);
				printCharAdr(v, 0x0F, X+21+c+((i+Y)*80));
			}
		}
	}

	return;
}*/

//i'm, sick and tired of pasting that one snippet of code over and over to calculate x y values from videoStart. Here's a function to do that
void cursorAdrToInts(int *x, int *y)
{
	unsigned int X = (((unsigned int)videoStart-0xB8000)/2) % 80;
	unsigned int Y = (((unsigned int)videoStart-0xB8000)/160);

	*x = X;
	*y = Y;

	return;
}

//fuck it. 0 = intel. 1 = amd. 2 = cyrix. 3 = ibm
string cpuModelToString(int manufacturer, int model, int family)
{
	string vendorName;
	string modelName;

	if (family == 4)
	{
		//486 dx1
		if (model >= 0 && model < 2)
		{
			modelName = "486DX";
		}
		else if (model == 2)
		{
			modelName = "486SX";
		}
		else if (model == 3 || model == 7)
		{
			modelName = "486DX2";
		}
		else if (model == 4)
		{
			modelName = "486SL";
		}
		else if (model == 5)
		{
			modelName = "486SX2";
		}
		else if (model == 8 || model == 9)
		{
			modelName = "486DX4";
		}
		else if (model == 0xE || model == 0xF)
		{
			modelName = "5x86 DX5";
		}
	}
	else if (family == 5)
	{
		if (manufacturer == 0)
		{
			if (model < 4)
			{
				modelName = "Pentium";
			}
			else if (model >= 4)
			{
				modelName = "Pentium MMX";
			}
		}
		else
		{
			if (model < 4)
			{
				modelName = "K5";
			}
			else if (model ==6 || model == 7)
			{
				modelName = "K6";
			}
			else if (model == 8)
			{
				modelName = "K6-II";
			}
			else if (model == 9)
			{
				modelName = "K6-III";
			}
			else if (model == 0xD)
			{
				modelName = "K6-III+";
			}
		}
	}
	else if (family == 6)
	{
		//if intel
		if (manufacturer == 0)
		{
			if (model < 2)
			{
				modelName = "Pentium Pro";
			}
			else if (model < 7)
			{
				modelName = "Pentium II";
			}
			else if (model < 9 || model == 0xA)
			{
				modelName = "Pentium III";
			}
			else if (model == 5 || model == 6)
			{
				modelName = "Pentium 4";
			}
			else
			{
				modelName = "Pentium M";
			}
		}
		//AMD
		else
		{
			if (model < 6 && model != 3)
			{
				modelName = "Athlon";
			}
			else if (model == 7 || model == 3)
			{
				modelName = "Duron";
			}
			else
			{
				modelName = "Athlon XP";
			}
		}
	}
	//one more level just for fun
	else if (family == 0xF)
	{
		if (manufacturer == 0)
		{
			modelName = "Pentium 4";
		}
		else
		{
			if (model == 5)
			{
				modelName = "Opteron";
			}
			else
			{
				modelName = "Athlon 64";
			}
		}
	}
	//the special cases
	else
	{
		if (family == 0x15 && model == 3)
		{
			modelName = "Pentium Overdrive";
		}
		modelName = "something modern";
	}

	//now do the vendor stuff
	switch (manufacturer)
	{
	case 0:
		vendorName = "Intel ";
		break;
	case 1:
		vendorName = "AMD ";
		break;
	case 2:
		vendorName = "Cyrix ";
		break;
	case 3:
		vendorName = "VIA ";
		break;
	case 4:
		vendorName = "SIS ";
		break;
	case 5:
		vendorName = "UMC ";
		break;
	case 6:
		vendorName = "Vortex 86 ";
		break;

	default:
		vendorName = "unknown vendor. ";
		break;
	}

	string finalString = vendorName + modelName;
	return finalString;
}

//fuck it. 0 = intel. 1 = amd. 2 = cyrix. 3 = via. 4 = sis. 5 = umc. 6 = vortex86
int getCpuVendor()
{
	int result = cpu_vendor();

	//"Genu" - it's probably intel
	if (result == 0x756E6547)
	{
		return 0;
	}
	//both amd strings
	else if (result == 0x68747541 || result == 0x69444D41)
	{
		return 1;
	}
	//cyrix
	else if (result == 0x69727943)
	{
		return 2;
	}
	//VIA
	else if (result == 0x20414956)
	{
		return 3;
	}
	//sis
	else if (result == 0x20534953)
	{
		return 4;
	}
	//umc
	else if (result == 0x20434D55)
	{
		return 5;
	}
	//vortex 86
	else if (result == 0x73726F56)
	{
		return 6;
	}
	return 7;
}

void printMemoryManagement()
{
	//fucking strings fuckity fuck fuck
	printString("Total used memory is ", 0x0F);
	printInt(totalUsedMemory(), 0x0E);
	printString(" out of ", 0x0F);
	printInt(totalMaxMemory(), 0x0E);
	//messageString += totalUsedMemory()
}

void printMemInfo(bool verbose)
{
    for (int i = 0; i < currentSize; i++)
    {
        printInt(mmt[i].m_start, 0x0F, false);
        printChar('-',0x0F);
        printInt(mmt[i].m_end, 0x0F, false);
        printChar(' ',0x0F);
        printChar(' ',0x0F);
        printInt(mmt[i].m_end - mmt[i].m_start, 0x0F, false);
		if (verbose)
		{
			for (int u = 0; u < 40; u++)
			{
				printChar(*(char*)(mmt[i].m_start + u), 0x0D);
			}
		}
        consoleNewLine(3);
    }
	//printMemoryManagement();
	printInt(currentSize, 0x0E, false);
}