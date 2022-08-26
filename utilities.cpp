//figure out how much memory the system has
void memoryTest(bool printToScreen, unsigned int startAddress, unsigned int kbToTest)
{
	bool success = true;
	u_int8_t* currentAddress = (u_int8_t*)startAddress;
	//print32bitNumber(startAddress, 100);
	while (success && ((unsigned int)currentAddress - (unsigned int)startAddress)/1000 < kbToTest)
	{
		//first, set the address to zero then check if it's zero when read
		*currentAddress = 0x00;
		u_int8_t read = *(u_int8_t*)currentAddress;
		if (read != 0x00)
		{
			success = false;
		}

		//next, set the address to 255 then check to see if it changed to 255
		*currentAddress = 0xFF;
		read = *(u_int8_t*)currentAddress;
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

//prints the memory address of all occurances of the word "fuck" in the given memory range 
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

// OK: literal operators can be overloaded
//doesnt solve the string issue though
char* operator ""_Z(const char* args, size_t csize)
{
	//void* ptr = calloc()
	//char currentChar = *(char*)args;
	int size = csize;
	/*while (currentChar != 0)
	{
		size++;
		currentChar = args[size];
	}*/
	void* ptr = calloc(size, sizeof(char));

	char currentChar = *(char*)args;
	int i = 0;
	while (i < size)
	{
		*(char*)(ptr+i) = args[i];
		i++;
	}

	return (char*)ptr;

}