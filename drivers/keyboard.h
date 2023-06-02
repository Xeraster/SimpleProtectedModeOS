#ifndef KEYBOARD_H
#define KEYBOARD_H

//most of the things in here are nearly direct translations of the keyboard driver on my real mode version of this system

//returns true if success, false if timed out
bool keyboardWaitReadReady();

//returns true if success, false if timed out
bool keyboardWaitWriteReady();

//waits for acknoledge signal from keyboard.
//returns true is success, false if timed out
bool keyboardWaitAck();

//waits for user input and 
char waitChar();

//deals with stuff related to special keyboard functions (i.e. caps lock or shift)
bool manageSpecialKeyboardFunctions(char function, bool lastKeyAlt);

/*converts inputted scancode into a character
0 = original XT scancode set
1 = PS/2 scan code set for AT motherboards
2 = default scan code set for "modern" keyboards
*/
char scancodeToChar(char scancode, unsigned int scancodeSet);

//anything other than xt scancode set isn't supported right now. Note that HT6542B ps/2 controllers default to "modern" keyboard scancode set but others default to xt
void setupScancodeMap();

//sets the keyboard status lights to whatever you want them to be
void setKeyboardLeds(bool caps, bool num, bool scroll);

//assembly functions. I made these because I thought they would make things not crash on
//non-emulated systems but it turns out it doesn't make a difference
extern "C" void waitAck();

extern "C" void kbd8042WaitReadReady();

extern "C" void kbd8042WaitWriteReady();

extern "C" void asmOutb(char byte, short port);

extern "C" char asmInb(short port);

#include "keyboard.cpp"

#endif