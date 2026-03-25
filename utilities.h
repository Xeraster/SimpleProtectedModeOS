#ifndef UTILITIES_H
#define UTILITIES_H

//figure out how much memory the system has
void memoryTest(bool printToScreen, unsigned int startAddress, unsigned int kbToTest = 1000);

//takes an 8 bit value and prints it to the screen
void print8bitNumber(unsigned int num, unsigned int curPos);

//takes an 16 bit value and prints it to the screen
void print16bitNumber(unsigned int num, unsigned int curPos);

//takes an 32 bit value and prints it to the screen
void print32bitNumber(unsigned int num, unsigned int curPos);

//slower than the other number printing functions but more versatile and it doesn't have leading zeros
void printAnyNumber(unsigned int num, short color, unsigned int curPos);

//(apparently they do) adds 2 numbers together then returns the result
int testIfReturnTypesWork(int num1, int num2);

void setCharEqual(char *target, unsigned int *poop);

//prints the memory address of all occurances of the word "fooo" in the given memory range 
void findFooo(unsigned int memStart, unsigned int length, int curPos);

//shifts the cursor to the left or right. Positive values shift right. Negative values shift left.
void shiftCursor(int howFar);

//performs the equivalent of backspace on textmode 7 or 3 or whatever the default one is
void backspaceScreen();

//void printMemoryAllocation(bool contents = false);

//i'm, sick and tired of pasting that one snippet of code over and over to calculate x y values from videoStart. Here's a function to do that
void cursorAdrToInts(int *x, int *y);

string cpuModelToString(int manufacturer, int model, int family);

int getCpuVendor();

void printMemoryManagement();

void printMemInfo(bool verbose = false);

//print the number of memory allocated things in ram
void printMemNum();

//THE HEX READER. Prints 256 bytes to the screen starting at given address. A useful hex reader
void readMemoryHex(unsigned int address, unsigned int rows = 16);

#include "utilities.cpp"

#endif