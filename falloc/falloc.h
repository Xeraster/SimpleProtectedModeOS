//Free Ultimate Commented Casual Allocator
//aka the FUCC Allocator
//allocates memory
//no outside libraries. at all
//easily configurable making it suitable for uncommon use cases
#ifndef FALLOC_H
#define FALLOC_H

//where in memory things should start
const unsigned int MEMORY_LOC = 0x120000;//0x30004;    //seems like a good spot maybe
const unsigned int START_TABLE_SIZE = 4096; //absolute size of the starting table in bytes. Keep in mind memData structs are 8 (i think) bytes in size
unsigned int MEMORYLIMIT = 0xFFFFFF;//where is the ram limit cutoff. non-const because you are intended be able to change this on the fly
const unsigned int CONFLICT_CHECK_STEPPING = 4;//the higher this number is, the faster but less stable the memory manager runs
//*hits blunt* what if instead of checking all 2^32 bytes for a free spot, you check +1 byte beyond what block n is, and repeat until you either get past the last block or find a suitable free space?
//expanding can't happen without a complete rewrite of the memory system, but having good defragmentation makes up for it. 
//If I need block expansion AND defragmentation, i'd rather spend effort improving the hardware so I can run the Linux kernel insted, a route that would probably be easier in addition to having better end results

struct memData
{
    unsigned int m_start;
    unsigned int m_end;
};

//memory metadate table. Where all the start addresses and stop addresses for all the memory is stored
memData *mmt;
unsigned int currentSize = 0;//housekeeping size variable
//unsigned int mainIndex = -1;//this is hard to wrap your head around but this is the index of where in the memeory metadata table contains the memory entry for the memory metadata table

//to make all the complicated functions less complicated, a startup sequence must be run to initialize the memory manager
void initialize();

//programmer only has to worry about these allocation functions
void *malloc(unsigned int size);
void *realloc(void *ptr, unsigned int size);
void *calloc(unsigned int num, unsigned int size);
void free(void *ptr);
void *memset(void *ptr, int value, unsigned int num);

//no segmentation. no defragging
/*
for i = 0
if memorySpot[0].

*/

//resize the memory metadata array. returns the location of the new memory metadata array
void resizeArray(unsigned int newSize);

//remove an item from memory metadata array at provided position
void removeItem(unsigned int index);

//adds and item to the memory metadata table
void* addItem(memData newitem);

//returns the size of the memory metadata table
unsigned int getMaxSizeMMT();

//find and return a memdata object with the same start number as the array
memData *findMMU();

//returns the first position in memory that is unallocated and of the correct size
void *lowestUnusedPosition(unsigned int size);

//the same as the "old" inefficient way except that it starts at the last memory block and THEN checks every byte in memory. Hopefully this will improve speed
void *lowestUnusedPositionStartingFromEndOfMemoryAllocation(unsigned int size, unsigned int startAddress);

//returns the first position in memory that is unallocated and of the correct size the slow but non-buggy way
void* lowestUnusedPositionOld(unsigned int size);

void memFill(void *dst, char c, unsigned int len);

bool conflicts(unsigned int sadr1, unsigned int eadr1, unsigned int sadr2, unsigned int eadr2);

//returns true if the provided start and end address block conflicts with anything in the memory allocation table
bool anyconflict(unsigned int start, unsigned int end);

//if ptr is a memory block, return its size. if not, it will return -1
unsigned int getSizeOfPointer(void *ptr);

unsigned int getIndexOfPointer(void *ptr);

//returns total number of bytes of allocated memory.
unsigned int totalUsedMemory();

//returns total amount of available memory. simple getter
unsigned int totalMaxMemory()
{
    return MEMORYLIMIT - MEMORY_LOC;
}

//returns true if there is a memory allocation entry that starts at the specified address. returns false if otherwise
bool allocationExists(void *ptr);

#include "falloc.hpp"

#endif
