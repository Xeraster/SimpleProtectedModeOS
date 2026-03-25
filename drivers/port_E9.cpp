void E9_stringCreateDebug(unsigned int var1, char var2, void* ptr)
{
    //make it as error avoidant as possible
	asmOutb('[', 0xE9);
	asmOutb(var2, 0xE9);
	asmOutb(']', 0xE9);
    asmOutb('c', 0xE9);
    asmOutb('r', 0xE9);
    asmOutb('e', 0xE9);
    asmOutb('a', 0xE9);
    asmOutb('t', 0xE9);
    asmOutb('e', 0xE9);
    asmOutb('d', 0xE9);
    asmOutb(' ', 0xE9);
    asmOutb('s', 0xE9);
    asmOutb('t', 0xE9);
    asmOutb('r', 0xE9);
    asmOutb('i', 0xE9);
    asmOutb('n', 0xE9);
    asmOutb('g', 0xE9);
    asmOutb(' ', 0xE9);
    asmOutb('o', 0xE9);
    asmOutb('f', 0xE9);
    asmOutb(' ', 0xE9);
    asmOutb('s', 0xE9);
    asmOutb('i', 0xE9);
    asmOutb('z', 0xE9);
    asmOutb('e', 0xE9);
    asmOutb(' ', 0xE9);
    intToE9(var1, false);
	asmOutb(' ', 0xE9);
	asmOutb('a', 0xE9);
	asmOutb('t', 0xE9);
	asmOutb(' ', 0xE9);
	asmOutb('p', 0xE9);
	asmOutb('o', 0xE9);
	asmOutb('s', 0xE9);
	asmOutb('=', 0xE9);
	intToE9((unsigned int)ptr, false);

	//newline at the end for serial readability
	asmOutb('\n', 0xE9);
}

void E9_stringDeleteDebug(unsigned int var1, char var2, void* ptr)
{
	asmOutb('[', 0xE9);
	asmOutb(var2, 0xE9);
	asmOutb(']', 0xE9);
	asmOutb('d', 0xE9);
	asmOutb('e', 0xE9);
	asmOutb('l', 0xE9);
	asmOutb(' ', 0xE9);
	asmOutb('s', 0xE9);
	asmOutb('t', 0xE9);
	asmOutb('r', 0xE9);
	asmOutb('i', 0xE9);
	asmOutb('n', 0xE9);
	asmOutb('g', 0xE9);
	asmOutb(' ', 0xE9);
	asmOutb('s', 0xE9);
	asmOutb('i', 0xE9);
	asmOutb('z', 0xE9);
	asmOutb('e', 0xE9);
	asmOutb(' ', 0xE9);
	intToE9(var1, false);
	asmOutb(' ', 0xE9);
	asmOutb('a', 0xE9);
	asmOutb('t', 0xE9);
	asmOutb(' ', 0xE9);
	intToE9((unsigned int)ptr, false);
	//newline at the end for serial readability
	asmOutb('\n', 0xE9);
	asmOutb(' ', 0xE9);
	asmOutb(' ', 0xE9);
	asmOutb(' ', 0xE9);
	E9_attemptToPrintData(ptr, var1);
	asmOutb('\n', 0xE9);

}

void E9_dynarrayDelete(void* ptr, unsigned int len)
{
	asmOutb('d', 0xE9);
	asmOutb('e', 0xE9);
	asmOutb('l', 0xE9);
	asmOutb(' ', 0xE9);
	intToE9((unsigned int)ptr, false);
	asmOutb(' ', 0xE9);
	asmOutb('o', 0xE9);
	asmOutb('f', 0xE9);
	asmOutb(' ', 0xE9);
	asmOutb('l', 0xE9);
	asmOutb('e', 0xE9);
	asmOutb('n', 0xE9);
	asmOutb('=', 0xE9);
	intToE9((unsigned int)len, false);
	asmOutb('\n', 0xE9);
}

void E9_debug3()
{
	asmOutb('d', 0xE9);
	asmOutb('e', 0xE9);
	asmOutb('b', 0xE9);
	asmOutb('u', 0xE9);
	asmOutb('g', 0xE9);
	asmOutb('3', 0xE9);
	asmOutb('(', 0xE9);
	asmOutb(')', 0xE9);
	asmOutb('\n', 0xE9);
	asmOutb('\n', 0xE9);
}

void E9_strcpy(void* ptr)
{
	asmOutb('c', 0xE9);
	asmOutb('o', 0xE9);
	asmOutb('p', 0xE9);
	asmOutb('y', 0xE9);
	asmOutb(' ', 0xE9);
	asmOutb('s', 0xE9);
	asmOutb('t', 0xE9);
	asmOutb('r', 0xE9);
	asmOutb('\n', 0xE9);
}

//try to print contents of given memory segment
void E9_attemptToPrintData(void* ptr, unsigned int len)
{
	for (int i = 0; i < len; i++)
	{
		char yeah = *(char*)(ptr + i);
		asmOutb(yeah, 0xE9);
	}
}

void intToE9(unsigned int number, bool hex)
{
	char printableResult[10]; //designate 5 spaces for the maximum sized 16 bit integer

	//use a different encoding scheme for hex and decimal
	if (hex)
	{
		//convert each digit to single chars
		printableResult[0] = (number & 0xF0000000)>>28;
		printableResult[1] = (number & 0x0F000000)>>24;
		printableResult[2] = (number & 0x00F00000)>>20;
		printableResult[3] = (number & 0x000F0000)>>16;
		printableResult[4] = (number & 0x0000F000)>>12;
		printableResult[5] = (number & 0x00000F00)>>8;
		printableResult[6] = (number & 0x000000F0)>>4;
		printableResult[7] = number & 0x0000000F;

		//for each char, convert to hex represensation
		for (int i = 0; i < 8; i++)
		{
			printableResult[i] = E9_intToHexChar(printableResult[i]);
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			char numPow = ((number/(E9_pow_slow(10, i)))%10)+48; //num / 10 power i
			printableResult[9-i] = numPow;
		}
	}

	bool foundFirstDigit = false;
	for (int i = 0; i < 10; i++)
	{
		if (printableResult[i] != 48)
		{
			foundFirstDigit = true;
		}

		if(foundFirstDigit)
		{
			//print the digit to the screen
			//printChar(printableResult[i], color);
            asmOutb(printableResult[i], 0xE9);
		}
	}

	//edge case for if the inputted number = 0
	if (number == 0) asmOutb('0', 0xE9);

	return;

	/*printableResult[0] = (num % 10) + 48;		least significant digit
	printableResult[1] = ((num / 10) % 10) + 48;
    printableResult[2] = ((num / 100) % 10) + 48;
	printableResult[3] = ((num / 1000) % 10) + 48;
	printableResult[4] = ((num / 10000) % 10) + 48; 
	printableResult[5] = ((num / 100000) % 10) + 48; 
	printableResult[6] = ((num / 1000000) % 10) + 48; 
	printableResult[7] = ((num / 10000000) % 10) + 48; 
	printableResult[8] = ((num / 100000000) % 10) + 48; 
	printableResult[9] = ((num / 1000000000) % 10) + 48; most significant digit

	printCharAdr(printableResult[0], 0x0F, curPos + 9);
	printCharAdr(printableResult[1], 0x0F, curPos + 8);
    printCharAdr(printableResult[2], 0x0F, curPos + 7);
	printCharAdr(printableResult[3], 0x0F, curPos + 6);
	printCharAdr(printableResult[4], 0x0F, curPos + 5);
	printCharAdr(printableResult[5], 0x0F, curPos + 4);
	printCharAdr(printableResult[6], 0x0F, curPos + 3);
	printCharAdr(printableResult[7], 0x0F, curPos + 2);
	printCharAdr(printableResult[8], 0x0F, curPos + 1);
	printCharAdr(printableResult[9], 0x0F, curPos);*/
}

void E9_printMemoryAt(void *location, unsigned int len)
{
	for (int m = 0; m < len; m+=4)
	{
		//split memory data node apart for easier reading on the e9 console
		if (m % 32 == 0)
		{
			asmOutb('\n', 0xE9);
			asmOutb('\n', 0xE9);
		}
		else if (m % 16 == 0)
		{
			asmOutb('\n', 0xE9);
		}
		unsigned int data = *(unsigned int*)(location + m);
		intToE9(data, false);
		asmOutb(' ', 0xE9);

	}

	asmOutb('\n', 0xE9);
}