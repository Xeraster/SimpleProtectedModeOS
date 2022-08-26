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

//prints the memory address of all occurances of the word "fuck" in the given memory range 
void findFooo(unsigned int memStart, unsigned int length, int curPos);

// OK: literal operators can be overloaded
char* operator ""_Z(const char* args, size_t csize);

#include "utilities.cpp"

#endif