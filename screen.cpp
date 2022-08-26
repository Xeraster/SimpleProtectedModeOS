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
	for (int i = 0; i < 7; i++)
	{
		printCharAdr(text.at(i),color, curpos+i);
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