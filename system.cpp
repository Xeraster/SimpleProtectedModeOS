//#pragma SECTION .kernel
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
	//*(char*)0xB8006 = 'E';
	//*(char*)0xB8007 = 0x0E;

	//start running the program
	_start();
}

//int 3h doesn't make bochs generate breakpoints for me but using nm system.o to get the location of this function and using the pbreak command to manually define the breakpoint address is good enough
void breakpointHack();

void breakpointHack()
{
	return;
}


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

//runs cpuid with eax=1 and then returns result which will be in eax register
extern "C" int cpu_ident();
extern "C" int cpu_vendor();
extern "C" void insert_breakpoint_asm();//surprise surprise. the breakpoint feature that never works, doesn't work

extern "C" void asmOutb(char byte, short port);

extern "C" char asmInb(short port);

//output to port 16 bits
extern "C" void asmOutW(short word, short port);

//input to port 16 bits
extern "C" short asmInW(short port);

//runs the memory allocator through a rigorous test. If the system doesn't crash, success
bool memManagementTest(bool test1, bool test2, bool test3);

//does memory management work? Try an rarray test
bool rarrayTest(bool test1 = true, bool test2 = true);

//the ultimate testament of stability
bool stringTest();

//#include <cmath> //apparently this is required for u_int8_t and shockingly, it actually works

//this also actually works for some reason both on emulators and real hardware
#include <sys/io.h>
#include <cstdint>
#include "drivers/port_E9.h"

//dynarrays are just a custom vector-like class
//#include "data types/dynarray.h"

//custom string class that works on dynarrays. not the same as std string but a lot of the functionality is the same
//#include "data types/string.h"

//math stuff
#include "math/invsqrt.h"
#include "math/math.h"

//the dynamic array that keeps track of data block boundaries
//dynarray<unsigned int> memf;
//dynarray<unsigned int> memb;

//this is where the manually defined malloc, calloc, realloc, free and other things are
//#include "memory.h"
//does the same thing as memcpy. It trades speed for the advantage of being possibly less buggy and more reliable. i'm not sure if there ever was a bug where using slow_memcpy ever fixed anything though.
void *slow_memcpy(void *dst, const void *src, unsigned int len);

//copy memory from 1 location in ram to another location in ram
void *memcpy(void *dst, const void *src, unsigned int len);
//#include "liballoc/liballoc.c"
#include "falloc/falloc.h"
//#include "liballoc/linux.c"
#include "data types/rarray.h"
#include "data types/string.h"
#include "standard.h"			//std library equivalents
//char *videoStart;

//a basic date and time struct, for all your low-to-medium-effort date and time needs
struct datetime
{
	char hours;
	char minutes;
	char seconds;
	unsigned int year;		//years 0 - 4.2 billion. How's that for Y2038 compliance?
	char month;
	char day;
};

struct filesystemInfo
{
    //mbr stuff
    char partitionType;             //0x06 = FAT16. 0x0B, 0x0C is FAT32
    unsigned int numSectors;        //number of sectors in partition, for fat16 this number is only found in the mbr
    unsigned int startingLBA;       //the LBA address of the start of the partition

    //partition stuff
    unsigned int sectorsPerFat;
    short sectorsPerCluster;
    unsigned int bytesPerSector;     
    short reservedSectors;
    short fsInfoSectorLocation;
    unsigned int volumeSerialNumber;
    char numFats;

};

struct fileInfo
{
    bool isDirectory;       //true if directory, false if a file
    string fileName;
    string fileExtension;
    short clusterNumberLow;     //cluster number of file contents
    short clusterNumberHigh;    //cluster number of file contents
    unsigned int size;          //file size in bytes
    bool isValidFile = true;    //this entry should be ignored if this is not set to true
	datetime fileDateTime;		//can't remember if its date modified or date created, doesn't matter really

	~fileInfo()
	{
		fileName.~string();
		fileExtension.~string();
	}
};

//use this to keep track of partitions that have been mounted
struct dataVolumeInfo
{
	//string basicLabel;
	//unsigned int baseLBA;		//LBA address of the root folder
	filesystemInfo fsinfo;
	bool mounted = false;
	unsigned int currentDirectoryLBA;		//LBA of the parent directory. To "cd" to a new directory, set this number to the LBA of the contents of that file
	
};

#include "screen.h"
#include "utilities.h"

#include "math/md5.cpp"		//md5 finally works
#include "math/aes/aes.h"
#include "math/aes/aesUtils.h" //utilities to make aes easier to interface with c++ os

//uses the same scancode translation convention as my real mode system and my z80 system
string scancodesXT_lowercase;
string scancodesXT_uppercase;
bool capsLock;
bool numLock;
bool scrollLock;
#include "drivers/keyboard.h"
#include "drivers/pci.h"
#include "drivers/ata.h"
#include "drivers/vgadriver.h"
#include "drivers/tgssDriver.h"
#include "programs/paletteUtils.h"
#include "programs/graphics.h"
#include "programs/customerList/customerList.h"
#include "commands/commands.h"

//use this instead if there's ever a suspicion that memcpy causes crashes
void *slow_memcpy(void *dst, const void *src, unsigned int len)
{
    /*char *d = (char*)dst;
    char *s = (char*)src;
    for (int i = 0; i < len; i++)
    {
        d[i] = s[i];
    }

    return dst;*/
	//ok, time to start caring about performance
	return memcpy(dst,src,len);
}

//copy memory from 1 location in ram to another location in ram
void *memcpy(void *dst, const void *src, unsigned int len)
 {
         unsigned int i;
 
         /*
00023          * memcpy does not support overlapping buffers, so always do it
00024          * forwards. (Don't change this without adjusting memmove.)
00025          *
00026          * For speedy copying, optimize the common case where both pointers
00027          * and the length are word-aligned, and copy word-at-a-time instead
00028          * of byte-at-a-time. Otherwise, copy by bytes.
00029          *
00030          * The alignment logic below should be portable. We rely on
00031          * the compiler to be reasonably intelligent about optimizing
00032          * the divides and modulos out. Fortunately, it is.
          */
 
         if ((unsigned int)dst % sizeof(long) == 0 &&
             (unsigned int)src % sizeof(long) == 0 &&
             len % sizeof(long) == 0) {

                 long *d = (long*)dst;
                 const long *s = (long*)src;
 
                 for (i=0; i<len/sizeof(long); i++) {
                         d[i] = s[i];
                 }
         }
         else {
                 char *d = (char*)dst;
                 const char *s = (char*)src;
 
                 for (i=0; i<len; i++) {
                         d[i] = s[i];
                 }
         }
 
         return dst;
 }

void printStartup()
{
	printString("Welcome to Scott's Protected Mode operating system", 0x0F);
}

//10-26-23: ok, it's time to refine the interface to be less of a glitchy mess and more of a usable command line interface. 02/28/2025: wow if this is the "new" system, I'd hate to see what the old system was like
void _start(void)
{
	//forceIOPL_High();
	// give protected mode permissions to access io ports
	// ioperm(0, 0xFFFF, 0);

	//start address of vga textmode memory mapped io
	videoStart = (char*)0xB80A0;
	*(char*)0xB8006 = 'A';
	*(char*)0xB8007 = 0x0E;
	*(char*)0xB8008 = 'B';
	*(char*)0xB8009 = 0x0E;
	initialize();//initialized memory manager
	//addItem(test);
	*(char*)0xB800A = 'C';
	*(char*)0xB800B = 0x0E;
	void *ptr1 = malloc(16);
	void *ptr2 = malloc(4);
	void *ptr3 = malloc(50);
	//void *ptr4 = malloc(64);		//uncomment for the system to last longer before crashing. The C++ Bug business.
	E9_printMemoryAt((void*)0x20000, 64);
	intToE9(getIndexOfPointer(ptr1), false);
	//memManagementTest(false, false, true);
	//rarrayTest();
	//intToE9(currentSize, false);
	//rarray<char> fuck = rarray<char>();
	//fuck.push_back('4');
	//fuck.push_back('5');
	string poop = "poop";
	printString(poop, 0x0F);
	printString("..", 0x0D);
	printString(poop, 0x0E);
	
	//there is no memory stability torture test more difficult than setting up the stupidass scancode map.
	setupScancodeMap();
	stringTest();
	E9_printMemoryAt((void*)mmt, 64);
	*(char*)0xB800C = 'D';
	*(char*)0xB800D = 0x0D;
	asmOutb('\n', 0xE9);
	intToE9(getMaxSizeMMT(), false);
	asmOutb('\n', 0xE9);
	intToE9(findMMU()->m_start, false);
	asmOutb('\n', 0xE9);
	intToE9(findMMU()->m_end, false);
	asmOutb('\n', 0xE9);
	intToE9(currentSize, false);
	asmOutb('\n', 0xE9);
	asmOutW(0x3030, 0xE9);

	//printString("..325r2urff2ff22ff2f.", 0x0D);
	//printString(yahuh, 0x0F);
	//clear the screen. some/most (sample size 2) bioses don't clear the screen before booting
	clearScreen(3);
	consoleNewLine(3);
	printStartup();
	consoleNewLine(3);
	memoryInfo("null");		//insert the memory info command just because reasons
	consoleNewLine(3);
	string commandBuffer = "";
	
	populateVgaDefaultTextmode();
	populateShittyAssVideoMode();

	//start the program loop
	while (3 == 3)
	{
		*(char*)0xB809A = 'Y';
		*(char*)0xB809B = 0x0D;

		char last = waitChar();
		*(char*)0xB809A = 'Z';
		*(char*)0xB809B = 0x0D;
		insertChar(last, 0xF);
		setVGAtextModeCursor(3);


		if (last > 31)
		{
			commandBuffer += last;
		}
		else if (last == 0x08)
		{
			//doesn't work right now
			//commandBuffer = commandBuffer.substr(0, commandBuffer.length() - 2);
			commandBuffer.pop();
			// commandBuffer += char(0);
			// commandBuffer = "";
			// commandBuffer = temps;
			last = 0;
		}

		*(char*)0xB809A = 'X';
		*(char*)0xB809B = 0x0D;

		int x, y;
		cursorAdrToInts(&x, &y);
		
		//no more screen spam
		/*for (int i = 0; i < 20; i++)
		{
			//print8bitNumber(*(char *)(commandBuffer.arrayAddress() + i), 240 + i);
			//char charToPrint = *(char *)(commandBuffer.arrayAddress() + i);
			char charToPrint = commandBuffer[i];
			printCharAdr(charToPrint, 0x0F, 240+i);
			// printInt(*(char *)(commandBuffer.arrayAddress() + i), 0x0F, true);
		}*/

		*(char*)0xB809A = 'C';
		*(char*)0xB809B = 0x0D;
		//print8bitNumber(commandBuffer.containerSize(), 62);

		setCurPos(x,y);

		//printString(commandBuffer, 0x05, 500);
		//print32bitNumber(commandBuffer.arrayAddress(), 580);

		*(char*)0xB809A = 'R';
		*(char*)0xB809B = 0x0D;

		//if user pressed enter key
		if (last == 10)
		{
			//recopy whatever is in command buffer to the screen to demonstrate memory allocator working correctly
			//if the memory allocation system is functioning any way other than 110% bug free, this will make it obvious
			//printString(commandBuffer, 0x0E);
			if (!parseCommand(commandBuffer))
			{
				consoleNewLine();
				printString("Invalid command. Type 'help'", 0x0E);
				//consoleNewLine(3);
				printMemInfo(true);
			}
			consoleNewLine();

			//clear the command buffer
			//commandBuffer.manual_delete();
			//commandBuffer = "";
			commandBuffer.manual_clear();
			// commandBuffer.manual_clear();
			*(char*)0xB8098 = 'l';//put in different position so i can determine if it ran this once or zero times just by looking at screen output
			*(char*)0xB8099 = 0x02;//green

		}

		*(char*)0xB809A = 'Y';
		*(char*)0xB809B = 0x0E;
	}

	return;
}

void _init_globals()
{

}

void commandLineLoop()
{

}

//runs the memory allocator through a rigorous test. If the system doesn't crash, success
bool memManagementTest(bool test1, bool test2, bool test3)
{
	//basic test
	if (test2)
	{
		void* *ptrs = (void**)malloc(500*sizeof(void*));//an array of pointers

		//allocate 500 memory spots of alternating sizes
		for (int i = 0; i < 500; i++)
		{
			ptrs[i] = malloc(4);
		}

		//now delete them all
		for (int i = 0; i < 500; i++)
		{
			free(ptrs[i]);
		}
	}

	//if something is going to fail, it's probably going to fail here
	if (test2)
	{
		//wow this one is so slow
		//now do it again using alternating sizes
		//too lazy to set up a pseudorandom number
		void* *ptrs2 = (void**)malloc(4121*sizeof(void*));//an array of pointers
		for (int i = 0; i < 1000; i++)
		{
			//irregular non 4 byte aligned sizes for maximum memory allocator stress
			unsigned int sizeToDo = 9;
			if (i % 4 == 0)
			{
				sizeToDo = 11;
			}
			else if (i % 4 == 1)
			{
				sizeToDo = 3;
			}
			else if (i % 4 == 2)
			{
				sizeToDo = 5;
			}
			else if (i % 4 == 3)
			{
				sizeToDo = 1;
			}

			ptrs2[i] = malloc(sizeToDo);
		}

		//if it survived all that, delete them all
		for (int i = 0; i < 4121; i++)
		{
			free(ptrs2[i]);
		}
	}

	//probably not as hard as test2 but it uses larger amounts of memory
	if (test3)
	{
		//allocate smaller numbers of larger data
		void* *ptrs3 = (void**)malloc(100*sizeof(void*));//an array of pointers

		//allocate 500 memory spots of alternating sizes
		for (int i = 0; i < 100; i++)
		{
			ptrs3[i] = malloc(4096);
		}

		//now delete them all
		for (int i = 0; i < 10; i++)
		{
			free(ptrs3[i]);
		}
	}

	//if it gets to this point, the test was a success
	return true;
}

//does memory management work? Try an rarray test
bool rarrayTest(bool test1, bool test2)
{
	//test1 is for primitive data types
	if (test1)
	{
		//try to break rarrays

		//simplest data types first
		rarray<char> firstArray = rarray<char>();
		rarray<char> secondArray = rarray<char>();
		intToE9(currentSize, false);
		asmOutb('\n', 0xE9);

		for (int i = 0; i < 20; i++)
		{
			firstArray.push_back('c');
		}

		for (int i = 0; i < 20; i++)
		{
			secondArray.push_back('u');
		}

		bool matches = false;
		int i = 0;
		while (!matches && i < 20)
		{
			if (firstArray.at(i) == secondArray.at(i))
			{
				matches = true;
			}
			i++;
		}

		//failed
		if (matches)
		{
			intToE9(currentSize, false);
			asmOutb('f', 0xE9);
			return false;
		}
	}

	//test2 is for complicated data types
	intToE9(currentSize, false);
	asmOutb('p', 0xE9);
	return true;
}

/*static __inline unsigned char inb (unsigned short int __port)
{
  unsigned char _v;
  __asm__ __volatile__ ("inb %w1,%0":"=a" (_v):"Nd" (__port));
  return _v;
}*/

//the ultimate testament of stability
bool stringTest()
{
	string test1 = "test1";
	string grapes = "grapes";
	string tttt;
	tttt = "tttt";

	string asses = grapes;
	asses += "_42";
	printString(test1, 0x01);
	printString(grapes, 0x02);
	printString(tttt, 0x03);
	printString(asses, 0x04);

	return true;
}