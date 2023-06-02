void _start(void);
void _init_globals();

void begin()
{
	//the very very very first thing it needs to do is write a character to the screen
	*(char*)0xB8004 = 'P';
	*(char*)0xB8005 = 0x0E;

	//initialize globals since that doesn't happen automatically
	//it *used* to not happen automatically. I still use this for initializing memory management and miscellaneous things though
	_init_globals();

	//if it made it this far, print another character to the screen
	*(char*)0xB8006 = 'E';
	*(char*)0xB8007 = 0x0E;

	//start running the program
	_start();
}

//int 3h doesn't make bochs generate breakpoints for me but using nm system.o to get the location of this function and using the pbreak command to manually define the breakpoint address is good enough
void breakpointHack();

void breakpointHack()
{
	return;
}

//where the usable ram space starts
unsigned int memorySpace; //start at the beginning of high memory space

//how many bytes of dynamic memory have been used
unsigned int numUsedBytes;

//how much memory is available in bytes
unsigned int totalMemory;

unsigned int programHeapStart;

unsigned int globalDevVar;

bool exitSignal;

bool capsLock;
bool numLock;
bool scrollLock;

void *__dso_handle;

//this doesn't do anything anymore so disregard it
//==========================================================================================
extern "C" void __cxa_pure_virtual()
{
    // Do nothing or print an error message.
}
extern "C" int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso)
{

}
extern "C" void __cxa_finalize(void *f)
{

}
//============================================================================================

//set bit 12-13 of IOPL to 11
extern "C" void forceIOPL_High();

//set bit 12-13 of IOPL to 00
extern "C" void forceIOPL_Low();

//manually defined memory management. works similarly to malloc, calloc and realloc in linux systems but not quite the same
extern void *malloc(unsigned int size);
extern void *calloc(unsigned int num_items, unsigned int size);
extern void *realloc(void *ptr, unsigned int size);
extern void free(void *ptr);

//when the user types stuff, store it in here
unsigned int commandBufferAddress;

//#include <cmath> //apparently this is required for u_int8_t and shockingly, it actually works

//this also actually works for some reason both on emulators and real hardware
#include <sys/io.h>

//dynarrays are just a custom vector-like class
#include "data types/dynarray.h"

//custom string class that works on dynarrays. not the same as std string but a lot of the functionality is the same
#include "data types/string.h"

//math stuff
#include "math/invsqrt.h"
#include "math/math.h"

//the dynamic array that keeps track of data block boundaries
dynarray<unsigned int> memf;
dynarray<unsigned int> memb;

//this is where the manually defined malloc, calloc, realloc, free and other things are
#include "memory.h"
#include "screen.h"
#include "utilities.h"

//uses the same scancode translation convention as my real mode system and my z80 system
string scancodesXT_lowercase;
string scancodesXT_uppercase;
#include "drivers/keyboard.h"
#include "drivers/pci.h"
#include "commands/commands.h"

void commandLineLoop();

void printWelcomeScreen();

void _start(void)
{
	//forceIOPL_High();
	// give protected mode permissions to access io ports
	// ioperm(0, 0xFFFF, 0);

	//start address of vga textmode memory mapped io
	videoStart = (char*)0xB8000;
	//clearScreen(3);
	//print8bitNumber(memf.getSize(), 94);

	//printTest();
	//printCharAdr('1', 0x0E, 1789);
	//printWelcomeScreen();

	//test the set cursor and integer printing functions
	setCurPos(0,6);
	printInt(0xF00F, 0x0F, true);

	int x, y;
	cursorAdrToInts(&x, &y);
	setCurPos(59,6);
	printMemoryAllocation();
	setCurPos(x,y);

	//test out basic floating point functionality
	string commandBuffer = "";
	printFloat(InvSqrt(4.0f), 4, true);
	consoleNewLine();
	printFloat(InvSqrt(16.0f), 4, true);
	consoleNewLine();
	printFloat(InvSqrt(412.114365f), 4, true);
	consoleNewLine();
	printFloat(InvSqrt(3241.116f), 4, true);
	consoleNewLine();

	//grab some bytes from the keyboard to demonstrate that this part actually does work
	printInt(inb(0x64), 0x0F);
	printInt(inb(0x60), 0x0F);
	printInt(inb(0x64), 0x0F);
	printInt(inb(0x60), 0x0F);

	while(!exitSignal)
	{
		//if it gets to this loop, print a character
		*(char*)0xB8094 = 'W';
		*(char*)0xB8095 = 0x01;//dark blue

		char last = waitChar();
		//if it gets to this loop, print a character
		*(char*)0xB8096 = 'W';
		*(char*)0xB8097 = 0x02;//green

		insertChar(last, 0xE);
		setVGAtextModeCursor(3);

		*(char*)0xB8096 = 'W';
		*(char*)0xB8097 = 0x03;//sky blue


		if (last > 31) commandBuffer+=last;

		*(char*)0xB8096 = 'W';
		*(char*)0xB8097 = 0x04;//red
		
		printCharArray("                 ", 0x0E, 62);
		int x, y;
		cursorAdrToInts(&x, &y);
		setCurPos(59,6);
		printMemoryAllocation();
		print8bitNumber(commandBuffer.length(), 77);
		commandBufferAddress = commandBuffer.arrayAddress();
		print32bitNumber(commandBufferAddress, 66);
		print8bitNumber(commandBuffer.containerSize(), 62);

		*(char*)0xB8096 = 'W';
		*(char*)0xB8097 = 0x05;//purple

		setCurPos(x,y);

		//if user pressed enter key
		if (last == 10)
		{
			parseCommand(commandBuffer);
			//printString(commandBuffer, 0x0E);
			consoleNewLine();

			//clear the command buffer
			//commandBuffer.manual_delete();
			commandBuffer = "";
			//commandBuffer.manual_clear();
			*(char*)0xB8098 = 'l';//put in different position so i can determine if it ran this once or zero times just by looking at screen output
			*(char*)0xB8099 = 0x02;//green

		}

		*(char*)0xB8096 = 'W';
		*(char*)0xB8097 = 0x06;//orange

		*(char*)0xB809A = 'Y';//put in different position so i can determine if it ran this once or zero times just by looking at screen output
		*(char*)0xB809B = 0x0E;//yellow

		//load a character to screen if it gets this far

	}

	printTest();


	return;
}

void _init_globals()
{
	videoStart = (char*)0xB8000;
	*(char*)0xB8006 = 'I';
	*(char*)0xB8007 = 0x0E;
	//initialize globals since that doesn't happen automatically
	memorySpace = 0x00100000;		//not used for anything right now
	numUsedBytes = 0;
	programHeapStart = 0x0007F000;
	totalMemory = 0x1000;
	*(char*)0xB8008 = 'G';
	*(char*)0xB8009 = 0x0E;

	//for now, dynamically allocated memory is between 0x7F000-0x80000
	memf = dynarray<unsigned int>(0x0007F000); //memory begin goes on the lower block
	*(char*)0xB800A = 'D';
	*(char*)0xB800B = 0x0E;
	memb = dynarray<unsigned int>(0x0007F800); //memory back goes on upper block
	*(char*)0xB800C = 'A';
	*(char*)0xB800D = 0x0E;

	//the memory for this is manually allocated since its a chicken and the egg problem. You can't allocate memory without having a memory allocation table. You can't have a memory allocation table without being able to allocate memory
	//thats why I made the dynarray. It solves that problem 
	memf.push_back(0x0007F000);
	memb.push_back(0x0007F000 + (memf.max_size()*sizeof(unsigned int)));
	//*(char*)0xB800E = 'M';
	//*(char*)0xB800F = 0x0E;
	setCurPos(1,1);
	printInt(memf.getSize(), 0x0E);
	//printChar('R', 0x0E);

	memf.push_back(0x0007F800);
	memb.push_back(0x0007F800 + memb.max_size()*sizeof(unsigned int));
	*(char*)0xB8010 = 'M';
	*(char*)0xB8011 = 0x0E;
	printInt(memb.getSize(), 0x0E);


	exitSignal = false;

	//memf.push_back(0x0007F800);
	//memb.push_back(0x0007F800 + (memb.max_size()*sizeof(unsigned int)));

	globalDevVar = 420;
	*(char*)0xB8012 = '4';
	*(char*)0xB8013 = 0x0E;

	//scancode data
	//scancodesXT_uppercase = (char*)calloc(88, sizeof(char));
	//scancodesXT_lowercase = (char*)calloc(88, sizeof(char));
	setupScancodeMap();

	*(char*)0xB8014 = 'S';
	*(char*)0xB8015 = 0x0E;
	//scancodesXT_lowercase = "poop1234567890-=qwertyuiop[]asdfghjkl;'z";
	//scancodesXT_lowercase + "o";
	//scancodesXT_lowercase+="teast5555555555zfafafa555555556";

	//start with these disabled by default
	capsLock = false;
	numLock = false;
	scrollLock = false;

	*(char*)0xB8016 = 'B';
	*(char*)0xB8017 = 0x0E;

	//commandBuffer = "";

	return;
}

void commandLineLoop()
{

}

void printWelcomeScreen()
{
	printCharArray("Welcome to Scott's protected mode operating system", 0x0F, 0);
	unsigned int usedMemory = 0;
	for (int i = 0; i < memf.getSize(); i++)
	{
		//usedMemory += getSizeOfMemBlock((void*)memf.at(i), nullptr);
		usedMemory += getSizeOfMemBlock((void*)memf.at(i), 0);
	}

	/*can't really see this right that well because of all the debug output 
	that gets spammed at the top of the screen but right now it 
	doesn't get anywhere close to allocating more than 2kbytes of 
	memory which is currently the max it can do before it overflows*/
	printCharArray("There are ", 0x0F, 80);
	print32bitNumber(usedMemory, 90);
	printCharArray(" bytes in use.", 0x0F, 101);
	

}

/*static __inline unsigned char inb (unsigned short int __port)
{
  unsigned char _v;
  __asm__ __volatile__ ("inb %w1,%0":"=a" (_v):"Nd" (__port));
  return _v;
}*/