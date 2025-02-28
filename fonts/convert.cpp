//converts those shitty fonts i drew myself into a format that can be inserted into a c++ file
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

void readSection(string fileName, string *prefixes, int size);
void fileToCpp(string fileName, int size);

int main()
{
	fileToCpp("special1.raw", 16);
	fileToCpp("numbers.raw", 17);
	fileToCpp("capitalLetters.raw", 32);
	fileToCpp("lowercaseLetters.raw", 32);
	fileToCpp("special2.raw", 16);
	return 0;
	
	//old stuff leftover from when i used these fonts with my z80
	/*string *specialChars = new string[16];
	specialChars[0] = "space";
	specialChars[1] = "exclamation";
	specialChars[2] = "dquotes";
	specialChars[3] = "pound";
	specialChars[4] = "dollar";
	specialChars[5] = "percent";
	specialChars[6] = "ampersand";
	specialChars[7] = "quote";
	specialChars[8] = "lparenth";
	specialChars[9] = "rparenth";
	specialChars[10] = "asteri";
	specialChars[11] = "plus";
	specialChars[12] = "comma";
	specialChars[13] = "minus";
	specialChars[14] = "period";
	specialChars[15] = "fslash";

	string *numbers = new string[17];
	numbers[0] = "0";
	numbers[1] = "1";
	numbers[2] = "2";
	numbers[3] = "3";
	numbers[4] = "4";
	numbers[5] = "6";
	numbers[6] = "7";
	numbers[7] = "8";
	numbers[8] = "9";
	numbers[9] = "colon";
	numbers[10] = "semicolon";
	numbers[11] = "leftarrow";
	numbers[12] = "equal";
	numbers[13] = "rightarrow";
	numbers[14] = "question";
	numbers[15] = "email";
	numbers[16] = "error";

	string *capitalLetters = new string[32];
	capitalLetters[0] = "A";
	capitalLetters[1] = "B";
	capitalLetters[2] = "C";
	capitalLetters[3] = "D";
	capitalLetters[4] = "E";
	capitalLetters[5] = "F";
	capitalLetters[6] = "G";
	capitalLetters[7] = "H";
	capitalLetters[8] = "I";
	capitalLetters[9] = "J";
	capitalLetters[10] = "K";
	capitalLetters[11] = "L";
	capitalLetters[12] = "M";
	capitalLetters[13] = "N";
	capitalLetters[14] = "O";
	capitalLetters[15] = "P";
	capitalLetters[16] = "Q";
	capitalLetters[17] = "R";
	capitalLetters[18] = "S";
	capitalLetters[19] = "T";
	capitalLetters[20] = "U";
	capitalLetters[21] = "V";
	capitalLetters[22] = "W";
	capitalLetters[23] = "X";
	capitalLetters[24] = "Y";
	capitalLetters[25] = "Z";
	capitalLetters[26] = "halfsquare1";
	capitalLetters[27] = "backwardsslash";
	capitalLetters[28] = "halfsquare2";
	capitalLetters[29] = "idk2";
	capitalLetters[30] = "idk3";
	capitalLetters[31] = "alttilde";

	string *lowercaseLetters = new string[31];
	lowercaseLetters[0] = "a";
	lowercaseLetters[1] = "b";
	lowercaseLetters[2] = "c";
	lowercaseLetters[3] = "d";
	lowercaseLetters[4] = "e";
	lowercaseLetters[5] = "f";
	lowercaseLetters[6] = "g";
	lowercaseLetters[7] = "h";
	lowercaseLetters[8] = "i";
	lowercaseLetters[9] = "j";
	lowercaseLetters[10] = "k";
	lowercaseLetters[11] = "l";
	lowercaseLetters[12] = "m";
	lowercaseLetters[13] = "n";
	lowercaseLetters[14] = "o";
	lowercaseLetters[15] = "p";
	lowercaseLetters[16] = "q";
	lowercaseLetters[17] = "r";
	lowercaseLetters[18] = "s";
	lowercaseLetters[19] = "t";
	lowercaseLetters[20] = "u";
	lowercaseLetters[21] = "v";
	lowercaseLetters[22] = "w";
	lowercaseLetters[23] = "x";
	lowercaseLetters[24] = "y";
	lowercaseLetters[25] = "z";
	lowercaseLetters[26] = "idk4";
	lowercaseLetters[27] = "pipeiguess";
	lowercaseLetters[28] = "idk5";
	lowercaseLetters[29] = "tilde";
	lowercaseLetters[30] = "errorchar";

	readSection("special1.data", specialChars, 16);
	readSection("numbers.data", numbers, 16);
	readSection("capitalLetters.data", capitalLetters, 32);
	readSection("lowercaseLetters.data", lowercaseLetters, 31);*/

}

void readSection(string fileName, string *prefixes, int size)
{
	char ch;
	ifstream myFile;
	myFile.open(fileName);
	string resultSoFar = "";
	bool getOut = false;
	int poop = 0;
	for (int i = 0; i < size; i++)
	{
		getOut = false;
		resultSoFar = "letters_" + prefixes[i] + ": db ";
		int y = 0;
		resultSoFar+="%";
		while (myFile && getOut == false)
		{
			myFile >> noskipws >> ch;
			poop++;
			if (ch == 0)
			{
				resultSoFar+="1";
			}
			else
			{
				resultSoFar+="0";
			}
			y++;
			if (y % 8 == 0 && y % 64 != 0)
			{
				resultSoFar+=", %";
			}
			if (y%64 == 0) 
			{
				getOut = true;
				y==0;
			}
		}
		cout << resultSoFar << endl;
		//cout << to_string(poop) << endl;
	}

	myFile.close();
}

//size = how many chars in file.
void fileToCpp(string fileName, int size)
{
	char ch;
	ifstream myFile;
	myFile.open(fileName);
	string resultSoFar = "";

	bool getOut = false;		//doesnt do anything? does stuff? idfk
	int poop = 0;		//i fucking forgot wtf this does

	//for each expected char in file, do this stuff
	//each char takes up 64 pixels at 1 bit per pixel
	//char *c = new char[1];
	//unsigned char *c = (unsigned char*)myFile.;
	unsigned char fuck;
	myFile >> fuck;

	vector<unsigned int> bitShit = vector<unsigned int>();
	for (int i = 0; i < size * 64; i++)
	{
		//cout << std::hex << (unsigned int)fuck << endl;
		//the bits need to be inverted because according to gimp, 0 = white and 1 = black. I want 0 = black and 1 = white
		if (fuck == 0 || fuck == 0xFF)//the numbers file is triggering a color indexing bug in GIMP and this is the workaround
		{
			bitShit.push_back(1);
		}
		else
		{
			bitShit.push_back(0);
		}
		myFile >> fuck;
	}

	//THERE, now its in such a form where I can do WHATEVER THE FUCK I WANT WITH IT
	//(im biased im sure, but why is this so much more complicated and difficult than it is in C++ OS?)
	vector<unsigned char> processedValues = vector<unsigned char>();
	for (int i = 0; i < size*64; i+=8)
	{
		if (i%64 == 0 && i != 0)
		{
			cout << endl;
		}

		unsigned char num = bitShit.at(i);
		num += bitShit.at(i+1)<<1;
		num += bitShit.at(i+2)<<2;
		num += bitShit.at(i+3)<<3;
		num += bitShit.at(i+4)<<4;
		num += bitShit.at(i+5)<<5;
		num += bitShit.at(i+6)<<6;
		num += bitShit.at(i+7)<<7;
		cout << "0x";
		if (num < 16)
		{	
			//add leading zeros to smaller numbers for cpmatibility
			cout << "0";
		}
		cout << std::hex << (unsigned int)num << ", ";
	}

	cout << endl;

	myFile.close();
}
