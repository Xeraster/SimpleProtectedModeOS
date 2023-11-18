#ifndef COMMANDS_H
#define COMMANDS_H

//deals with processing commands. Returns true if inputted thing was a valid command. Returns false if inputted command was something un-runnable
bool parseCommand(string input);

bool usedMemory(string input);

bool pciCheck(string input);

bool clearScreenCommand(string input);

bool colorTest(string input);

//substring very obviosuly showcases any problems with memory allocation
bool substringTest(string input);

bool printCpuInfo();

bool debugCommand1(string input);

bool debugCommand2(string input);

bool debugCommand3(string input);

bool memLeak(int size);

bool memoryInfo(string input);

bool stringSplitTest(string input);

//read an address and write it to the screen. kind of like a big read only hex editor
//bool readCommand(string input);

//bool writeCommand(string input);

//bool rCommand(string input);

#include "commands.cpp"

#endif