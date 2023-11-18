void printChar(char msg, short color)
{
	//only do it if the inputted character is a valid ascii code
	if (msg > 31)
	{
		*videoStart = msg;
		videoStart++;
		*videoStart = color;
		videoStart++;

	}

	return;
}

void printCharAdr(char msg, short color, unsigned int curPos)
{
	char* screen_address = (char*)(0xB8000 + (curPos*2));

	*screen_address = msg;
	screen_address++;
	*screen_address = color;

	return;

}

//prints a null terminated character array to screen
//returns number of spaces it printed out
int printCharArray(char* array, short color, unsigned int curPos)
{
	char charToPrint = *(char*)array;
	int i = 0;
	while (charToPrint != 0)
	{
		printCharAdr(charToPrint, color, curPos + i);
		i++;
		charToPrint = array[i];
	}

	return i;
}

void printString(string text, short color, unsigned int curpos)
{
	for (int i = 0; i < text.length(); i++)
	{
		printCharAdr(text.at(i),color, curpos+i);
	}

	return;
}

void printString(string text, short color)
{
	for (int i = 0; i < text.length(); i++)
	{
		printChar(text.at(i),color);
	}

	return;
}

//prints an int to the screen
void printInt(unsigned int number, short color, bool hex)
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
			printableResult[i] = intToHexChar(printableResult[i]);
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			char numPow = ((number/(pow_slow(10, i)))%10)+48; //num / 10 power i
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
			printChar(printableResult[i], color);
		}
	}

	//edge case for if the inputted number = 0
	if (number == 0) printChar('0', color);

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

void printFloat(float value, short color, bool hex)
{
	if (hex)
	{
		//just print floats to the screen in hex format for now
		int lol = *(unsigned int*)&value;
		printInt(lol, color, true);
	}
	else
	{

	}

	return;
}

void printTest()
{
	printChar('T', 0x0F);
	printChar('E', 0x0F);
	printChar('S', 0x0F);
	printChar('T', 0x0F);
    return;
}

//clears the screen to black
void clearScreen(int vgaMode)
{
	//if text mode 3, do the thing you have to do in order to clear the screen in text mode 3
	if (vgaMode == 3)
	{
		//there are 2048 character positions on a textmode 3 screen
		for (int i = 0; i < 2048; i++)
		{
			printCharAdr(0, 0x0, i);
		}
	}
	return;
}

//sets videoStart to the correct address for the inputted cursor position
void setCurPos(int posX, int posY)
{
	//set the address of video start to the desired position
	videoStart = (char*)(0xB8000 + ((posY * 80) + posX)*2);

	return;
}

void setVGAtextModeCursor(unsigned int posX, unsigned int posY, int vgaMode)
{
	/*3D4 = address
	//3D5 = data
	note that ega compatible and monochrome modes use different ports for this*/

	unsigned short cursorPosShort = posX + (posY*80); 

	//seperate them into high and low bytes
	unsigned short low = cursorPosShort&(0xFF);
	unsigned short high = cursorPosShort&(0x00FF);
	high>>=8;
	//send 0E to the address register
	outb(0x0E, 0x03D4);
	//send the cursor location high value
	outb(high, 0x3D5);

	//send 0F to the address register
	outb(0x0F, 0x03D4);
	//send the cursor location high value
	outb(low, 0x3D5);

	//set the cursor blink in front
	setTextModeAttribute(posX, posY, 0x0F, 3);

	//that should do it

	return;
}

void setVGAtextModeCursor(int vgaMode)
{
	/*3D4 = address
	//3D5 = data
	note that ega compatible and monochrome modes use different ports for this*/

	//seperate them into high and low bytes
	unsigned short low = (((unsigned int)videoStart)-0xB8000)/2;
	unsigned short high = low;
	low = low&(0xFF);
	low = high&(0x00FF);
	high>>=8;
	//send 0E to the address register
	outb(0x0E, 0x03D4);
	//send the cursor location high value
	outb(high, 0x3D5);

	//send 0F to the address register
	outb(0x0F, 0x03D4);
	//send the cursor location high value
	outb(low, 0x3D5);

	//print8bitNumber(1800, low);
	//print8bitNumber(1804, high);

	/*char printableResult[3];

	printableResult[0] = (low % 10) + 48;		//least significant digit
	printableResult[1] = ((low / 10) % 10) + 48; //middle digit
    printableResult[2] = ((low / 100)) + 48;  	//most significant digit
	printCharAdr(printableResult[0], 0x0F, 1800 + 2);
	printCharAdr(printableResult[1], 0x0F, 1800 + 1);
    printCharAdr(printableResult[2], 0x0F, 1800);

	printableResult[0] = (high % 10) + 48;		//least significant digit
	printableResult[1] = ((high / 10) % 10) + 48; //middle digit
    printableResult[2] = ((high / 100)) + 48;  	//most significant digit
	printCharAdr(printableResult[0], 0x0F, 1794 + 2);
	printCharAdr(printableResult[1], 0x0F, 1794 + 1);
    printCharAdr(printableResult[2], 0x0F, 1794);*/

	//set the cursor blinking to the text in front
	setTextModeAttribute(0x0F, 3);

	//that should do it

	return;
}

//set pixel attribute byte from a given input position
void setTextModeAttribute(unsigned int posX, unsigned int posY, short atr, int vgaMode)
{
	unsigned int curPos = (posY*80) + posX;
	char* atr_address = (char*)(0xB8000 + (curPos*2) + 1);
	
	*atr_address = atr;

	return;
}

//set pixel attribute byte based on whatever value is in the videoStart pointer
void setTextModeAttribute(short atr, int vgaMode)
{
	char* atr_address = videoStart + 1;
	*atr_address = atr;
	return;
}

//inserts a character on the screen at the specified position, shifting everything forwards 1 position
void insertChar(char msg, short color, int posX, int posY)
{
	//if the rest of the line is blank, just print a character normally without moving anything around
	if (restOfLineIsBlank(posX, posY))
	{
		printChar(msg, color);
	}
	//if the rest of the line isn't blank, move all the characters forward 1 and then print the char
	else
	{
		char *rowAdr = (char*)(0xB8000 + ((posY * 80))*2);
		for(int i = 159; i > (posX*2); i--)
		{
			*(rowAdr + i + 2) = *(char*)(rowAdr + i);
		}

		//everything has been shifting forward a little. Now, print the character to the now-empty spot
		printChar(msg, color);
	}
}

//inserts a character on the screen at the specified position, shifting everything forwards 1 position. Derives position automatically from videoStart
void insertChar(char msg, short color)
{
	//i'm not pasting this thing twice
	unsigned int X = (((unsigned int)videoStart-0xB8000)/2) % 80;
	unsigned int Y = (((unsigned int)videoStart-0xB8000)/160);

	insertChar(msg, color, X, Y);

	//videoStart+=2;

	return;
}

//returns true if everything to the left of the given position up until position 80 is blank
bool restOfLineIsBlank(int posX, int posY)
{
	unsigned int curPos = (posY*80) + posX;
	char* atr_address = (char*)(0xB8000 + (curPos*2));

	bool isBlank = true;
	int i = posX;
	while(i < 80 && isBlank)
	{
		if (*atr_address > 30)
		{
			isBlank = false;
		}
		atr_address+=2;
		i++;
	}

	return isBlank;
}

void consoleNewLine(unsigned int videoMode)
{
	//convert video start pointer to individual x y values
	unsigned int X = (((unsigned int)videoStart-0xB8000)/2) % 80;
	unsigned int Y = (((unsigned int)videoStart-0xB8000)/160);

	setCurPos(0, Y + 1);
	setVGAtextModeCursor(3);

	//if there are less than 2 "blank" lines under this one worth of screen space left, move the screen up by 1
	if (Y > 21)
	{
		unsigned int origAdr = (unsigned int)videoStart;  //save the cursor address so it can be restored later
		videoStart = (char*)0xB80A0;

		/*time for a for loop. 
		*/

		for (unsigned int i = (unsigned int)videoStart; i < 0xBFFFF; i++)
		{
			*(char*)(videoStart-160) = *(char*)videoStart;
			videoStart = (char*)i;
		}

		//wow that loop is always fairly complicated when I do it with raw assembly

		//be sure to put the original screen address pointer back to where it was
		setCurPos(0, Y);
		setVGAtextModeCursor(videoMode);
	}

	return;
}