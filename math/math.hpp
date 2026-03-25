int pow_slow(int input, int pow)
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

char intToHexChar(int input)
{
	char val = 0;
	if (input < 10) val=input+48;
	else if (input < 16) val = input + 55;
	else val = 78;

	return val; 
}

unsigned int betterRand(unsigned int seed)
{
	//first time ever using long long int
	seed += 0xe120fc15;
	unsigned long long int tmp;
	tmp = (unsigned long long int)seed * 0x4a39b70d;
	unsigned int m1 = (tmp >> 32) ^ tmp;
	tmp = (unsigned long long int)m1 * 0x12fad5c9;
	unsigned int m2 = (tmp >> 32) ^ tmp;
	return m2;
}

//return the absolute value of the given number. Intended to be the same as the std math abs
unsigned int abs(int num)
{
	if (num < 0)
	{
		return num*-1;
	}
	else
	{
		return num;
	}

}