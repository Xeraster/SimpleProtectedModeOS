//don't put anything above this or it won't work. This is a required workaround I have to use because I couldn't get a linker to cooperate worth shit
//the compiler WILL spit out warnings about this. Just ignore it, it's fine.
void _start(void);
void _init_globals();
void begin()
{
	//initialize globals since that doesn't happen automatically
	_init_globals();

	//start running the program. Welcome to hell
	_start();
}

//put global system-wide variables below this line

//where the usable ram space starts
unsigned int memorySpace; //start at the beginning of high memory space

//how many bytes of dynamic memory have been used
unsigned int numUsedBytes;

//how much memory is available in bytes
unsigned int totalMemory;

unsigned int programHeapStart;

unsigned int globalDevVar;

#include <cmath> //apparently this is required for u_int8_t and shockingly, it actually works
//using namespace std;
#include "data types/dynarray.h"
#include "data types/string.h"
//the dynamic array that keeps track of data block boundaries
dynarray<unsigned int> memf;
dynarray<unsigned int> memb;

#include "memory.h"
#include "screen.h"
#include "utilities.h"
#include <sys/io.h> //visual studio code flags this as an error. There is no way to make it shut up about it. It needs to shut the fuck up because it doesn't know what it's talking about


void _start(void)
{
	videoStart = (char*)0xB8000;
	clearScreen(3);
	char poop = inb(0x60);
	printTest();
	for (int i = 0; i < 16; i++)
	{
		printCharAdr('R' + i, i, 200+i);
	}
	printTest();
	char theWordFooo[] = "fooo11";
	findFooo(0, 0x80000, 140);
	//printString(testString, 0x0F, 1800);
	/*for (int i = 0; i < 7; i++)
	{
		printCharAdr(testString.at(i), 0x0F, 1800+i);
	}*/
	printCharArray(theWordFooo, 0x0F, 1800);

	print32bitNumber((unsigned int)theWordFooo, 81);
	//print32bitNumber(*(char*)"testmsg", 161);
	print8bitNumber(*(char*)theWordFooo, 241);
	//print8bitNumber(*(char*)(testmsg+2), 244);
	//print8bitNumber(*(char*)(testmsg+3), 247);
	//print8bitNumber(*(char*)(testmsg+4), 321);
	//print8bitNumber(*(char*)(testmsg+5), 324);
	//print8bitNumber(*(char*)(testmsg+6), 327);
	//print8bitNumber(*(char*)(testmsg+7), 330);
	//print8bitNumber(*(char*)pee+3, 247);
	//print8bitNumber(*(char*)pee+4, 250);
	//print8bitNumber(*(char*)pee+5, 253);
	//print8bitNumber(*(char*)pee+6, 256);

	print8bitNumber(memf.getSize(), 14);
	print8bitNumber(memb.getSize(), 17);
	for (int i = 0; i < memf.getSize(); i++)
	{
		print32bitNumber((unsigned int)memf.at(i), 92+(i*80));
		print32bitNumber((unsigned int)memb.at(i), 103+(i*80));
		printCharAdr('-',0x0F, 102+(i*80));
	}

	printTest();
	return;
}

void _init_globals()
{
	//initialize globals since that doesn't happen automatically
	memorySpace = 0x00100000;
	numUsedBytes = 0;
	programHeapStart = 0x0007F000;
	totalMemory = 0x1000;
	memf = dynarray<unsigned int>(0x0007F000); //memory begin goes on the lower block
	memb = dynarray<unsigned int>(0x0007F800); //memory back goes on upper block

	//the memory for this is manually allocated since its a chicken and the egg problem. You can't allocate memory without having a memory allocation table. You can't have a memory allocation table without being able to allocate memory
	memf.push_back(0x0007F000);
	memb.push_back(0x0007F000 + (memf.max_size()*sizeof(unsigned int)));

	memf.push_back(0x0007F800);
	memb.push_back(0x0007F800 + memb.max_size()*sizeof(unsigned int));

	//memf.push_back(0x0007F800);
	//memb.push_back(0x0007F800 + (memb.max_size()*sizeof(unsigned int)));

	globalDevVar = 420;

	return;
}

/*static __inline unsigned char inb (unsigned short int __port)
{
  unsigned char _v;
  __asm__ __volatile__ ("inb %w1,%0":"=a" (_v):"Nd" (__port));
  return _v;
}*/