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