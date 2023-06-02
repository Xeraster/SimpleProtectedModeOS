#ifndef SCREEN_H
#define SCREEN_H

char* videoStart;

void printChar(char msg, short color);

void printCharAdr(char msg, short color, unsigned int curPos);

//prints a null terminated character array to screen
//returns number of spaces it printed out
int printCharArray(char* array, short color, unsigned int curPos);

//print a string to any manually inputted position instead of whatever the value at videoStart is
void printString(string text, short color, unsigned int curPos);

//print a string using the value in videoStart instead of a cursor position input parameter
void printString(string text, short color);

//prints an int to the screen
void printInt(unsigned int number, short color, bool hex = false);

void printFloat(float value, short color, bool hex = false);

void printTest();

//clears the screen to black
void clearScreen(int vgaMode);

//sets videoStart to the correct address for the inputted cursor position
void setCurPos(int posX, int posY);

void setVGAtextModeCursor(unsigned int posX, unsigned int posY, int vgaMode = 3);

void setVGAtextModeCursor(int vgaMode = 3);

//set pixel attribute byte from a given input position
void setTextModeAttribute(unsigned int posX, unsigned int posY, short atr, int vgaMode = 3);

//set pixel attribute byte based on whatever value is in the videoStart pointer
void setTextModeAttribute(short atr, int vgaMode = 3);

//inserts a character on the screen at the specified position, shifting everything forwards 1 position
void insertChar(char msg, short color, int posX, int posY);

//inserts a character on the screen at the specified position, shifting everything forwards 1 position. Derives position automatically from videoStart
void insertChar(char msg, short color);

//returns true if everything to the left of the given position up until position 80 is blank
bool restOfLineIsBlank(int posX, int posY);

//move the cursor to the leftmost position of whatever line is under the current one
void consoleNewLine(unsigned int videoMode = 3);

#include "screen.cpp"

#endif
