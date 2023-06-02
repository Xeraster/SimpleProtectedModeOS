#ifndef COMMANDS_H
#define COMMANDS_H

//deals with processing commands. Returns true if inputted thing was a valid command. Returns false if inputted command was something un-runnable
bool parseCommand(string input);

bool usedMemory(string input);

bool pciCheck(string input);

#include "commands.cpp"

#endif