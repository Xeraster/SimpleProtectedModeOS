#ifndef SCREEN_H
#define SCREEN_H

char* videoStart;

void printChar(char msg, short color);

void printCharAdr(char msg, short color, unsigned int curPos);

//prints a null terminated character array to screen
//returns number of spaces it printed out
int printCharArray(char* array, short color, unsigned int curPos);

void printString(string text, short color, unsigned int curPos);

void printTest();

//clears the screen to black
void clearScreen(int vgaMode);

#include "screen.cpp"

#endif
