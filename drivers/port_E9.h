#ifndef PORT_E9
#define PORT_E9

void E9_stringCreateDebug(unsigned int var1, char var2, void* ptr);

void E9_stringDeleteDebug(unsigned int var1, char var2, void* ptr);

//try to print contents of given memory segment
void E9_attemptToPrintData(void* ptr, unsigned int len);

void E9_dynarrayDelete(void* ptr, unsigned int len);

void E9_debug3();

void intToE9(unsigned int number, bool hex = true);

void E9_strcpy(void* ptr);

char E9_intToHexChar(int input)
{
	char val = 0;
	if (input < 10) val=input+48;
	else if (input < 16) val = input + 55;
	else val = 78;

	return val; 
}

int E9_pow_slow(int input, int pow)
{
	int result = input;

	//anything to the power of zero is 1
	if (pow == 0) return 1;
	else
	{
		for (int i = 1; i < pow; i++)
		{
			result = result * input;
		}

		return result;
	}

}

void E9_printMemoryAt(void *location, unsigned int len);

#include "port_E9.cpp"

#endif