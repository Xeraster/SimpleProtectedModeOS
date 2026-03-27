//#pragma SECTION .kernel
void _start(void);
void _init_globals();

//0 = vga. 1 = v9958. 2 = tgss
int videoType = 0;
unsigned int tgss_cur_x = 0;//global tgss x cursor position until I make a better system for this
unsigned int tgss_cur_y = 0;//global tgss y cursor position until I make a better system for this
//convert font data to the correct format for the current bit depth and video mode and then draw it to the screen. This is the one that should be used for TGSS
extern void graphicsModeDrawText_tgss(unsigned int posX, unsigned int posY, short letter, unsigned short color, unsigned short backgroundColor, bool transparentBackground = false);

//set the tgss address pointer to the input value
extern void tgss_setAddressPointer(unsigned int address);

//check of the tgss is full or not and if not, waits until its not full if it is full
extern "C" void TGSS_waitNotFull();

//return a tgss vram addres pointer that is the equivalent for the given cursor position
extern unsigned int cursorPosToVramPos(unsigned int x, unsigned int y);

extern unsigned char vgafont8[256 * 8];

extern "C" void asmOutb(char byte, short port);

extern "C" char asmInb(short port);

//output to port 16 bits
extern "C" void asmOutW(short word, short port);

//input to port 16 bits
extern "C" short asmInW(short port);

extern "C" void setupPS2Controller();

extern "C" void configure_entry_point();

extern "C" void call_program();

const unsigned int KERNEL_VERSION_MAJOR = 1;
const unsigned int KERNEL_VERSION_MINOR = 0;

void begin()
{
	if (videoType == 0)
	{
		//the very very very first thing it needs to do is write a character to the screen
		*(char*)0xB8004 = 'P';
		*(char*)0xB8005 = 0x0E;
	}

	//update bios data rea hardware information
	unsigned char bdadata = *(unsigned char*)0x30D0;
	unsigned int video = (bdadata & 0x03);
	videoType = video;//there. video driver gets figured out automatically now

	//configure the system call enry point based on what BDA byte 0x30D0 bit 6 says about the system call entry point
	configure_entry_point();

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

//runs the memory allocator through a rigorous test. If the system doesn't crash, success
bool memManagementTest(bool test1, bool test2, bool test3);

//does memory management work? Try an rarray test
bool rarrayTest(bool test1 = true, bool test2 = true);

//the ultimate testament of stability
bool stringTest();

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
#include "drivers/v9958driver.h"
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

void _start(void)
{
	//forceIOPL_High();
	// give protected mode permissions to access io ports
	// ioperm(0, 0xFFFF, 0);
	asmOutb(0x00, 0xE9);
	//start address of vga textmode memory mapped io
	videoStart = (char*)0xB80A0;
	if (videoType == 0)
	{
		*(char*)0xB8006 = 'A';
		*(char*)0xB8007 = 0x0E;
		*(char*)0xB8008 = 'B';
		*(char*)0xB8009 = 0x0E;
	}
	asmOutb(0x00, 0xE9);
	initialize();//initialized memory manager
	asmOutb(0x00, 0xE9);
	void *ptr1 = malloc(16);
	void *ptr2 = malloc(4);
	void *ptr3 = malloc(50);

	if (videoType == 2)
	{
		//the only way this happens if on the special diy system. the keyboard controller wont have been turned on yet
		setupPS2Controller();
	}
	
	//there is no memory stability torture test more difficult than setting up the stupidass scancode map.
	setupScancodeMap();
	asmOutb(0x00, 0xE9);

	//clear the screen. some/most (sample size 2) bioses don't clear the screen before booting
	clearScreen(3);
	consoleNewLine(3);
	printStartup();
	consoleNewLine(3);
	memoryInfo("null");		//insert the memory info command just because reasons
	printCpuInfo(true);
	printVideoDriverInfo(videoType, true);
	consoleNewLine(3);
	//graphicsModeDrawText_tgss(3*8,3*8,'D', 0xF000, 0x0000, true);
	string commandBuffer = "";
	
	if (videoType == 0)
	{
		//vga cards need setup, the ice 40 fpga video card needs no setup
		populateVgaDefaultTextmode();
		populateShittyAssVideoMode();
	}
	//graphicsModeDrawText_tgss(2*8,3*8,'C', 0xFFFF, 0x0000, true);
	//start the program loop
	while (3 == 3)
	{
		//graphicsModeDrawText_tgss(1*8,3*8,'U', 0x0FFF, 0x0000, true);
		if (videoType == 0)
		{
			*(char*)0xB809A = 'Y';
			*(char*)0xB809B = 0x0D;
		}

		char last = waitChar();
		//graphicsModeDrawText_tgss(4*8,3*8,'B', 0x0FFF, 0x0000, true);
		if (videoType == 2)
		{
			*(char*)0xB809A = 'Z';
			*(char*)0xB809B = 0x0D;
		}

		if (videoType == 0)
		{
			insertChar(last, 0xF);
			setVGAtextModeCursor(3);
		}
		else
		{
			insertChar(last, 0xF);
			//printChar(last, 0x0F);
			//graphicsModeDrawText_tgss(5 * 8, 3 * 8, last, 0xFFFF, 0x0000, true);
		}


		if (last > 31)
		{
			commandBuffer += last;
		}
		else if (last == 0x08)
		{
			//commandBuffer = commandBuffer.substr(0, commandBuffer.length() - 2);
			commandBuffer.pop();
			// commandBuffer += char(0);
			// commandBuffer = "";
			// commandBuffer = temps;
			last = 0;
		}

		//*(char*)0xB809A = 'X';
		//*(char*)0xB809B = 0x0D;

		int x, y;
		cursorAdrToInts(&x, &y);

		setCurPos(x,y);

		//printString(commandBuffer, 0x05, 500);
		//print32bitNumber(commandBuffer.arrayAddress(), 580);

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
				//printMemInfo(true);
			}
			consoleNewLine();

			//clear the command buffer
			//commandBuffer.manual_delete();
			//commandBuffer = "";
			commandBuffer.manual_clear();
			// commandBuffer.manual_clear();
			if (videoType == 0)
			{
				*(char*)0xB8098 = 'l';//put in different position so i can determine if it ran this once or zero times just by looking at screen output
				*(char*)0xB8099 = 0x02;//green
			}

		}
		if (videoType == 0)
		{
			*(char*)0xB809A = 'Y';
			*(char*)0xB809B = 0x0E;
		}
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

/*system calls from assembly that were called from outside programs
*/
extern "C" void system_call_entry(unsigned int functionCode, void *data)
{
	if (functionCode == 0x00)//Get major kernel version
	{
		*(unsigned int*)data = KERNEL_VERSION_MAJOR;
		return;
	}
	else if (functionCode == 0x0001)//get minoir kernel version
	{
		*(unsigned int*)data = KERNEL_VERSION_MINOR;
		return;
	}
	else if (functionCode == 0x0002)//print the "it worked" message
	{
		printString("it worked!", 0x0F);
	}
	else if (functionCode == 0x0003)//print address of systemCall *data parameter
	{
		printString("data address = ", 0x0F);
		printInt((unsigned int)data, 0x0E);
	}
	else if (functionCode == 0x0004)//print a string
	{
		struct test
        {
            string str;
            short color;
        };
		test ccc = *(test*)data;
		printString(ccc.str, ccc.color);
	}
	else
	{
		printString("parameter corrupted = ", 0x0F);
		printInt(functionCode, 0x0E);
	}
}