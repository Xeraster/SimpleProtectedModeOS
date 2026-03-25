#ifndef MEMORY_H
#define MEMORY_H

extern void print32bitNumber(unsigned int num, unsigned int curPos);
/*
after a lot of thought, here is more or less how memory management needs to work

- there is a dynamic array of pointers containing end locations of allocated memory blocks
- malloc finds an empty spot in the provided memory space to allocate a block of data of the provided size. It then updates the pointer array to indicate where in memory the start and end of the block is
- free() deallocates memory starting at the inputted pointer address and ends when it reaches an address in the dynamic array of pointers
- the initial array is going to be somewhere in low memory space due to a memory allocation system not being ready upon initial startup but after its first allocation resulting in a resize, it will end up in high memory space (not yet implemented)

- this means we can find out not only exactly how much ram is being used, but how many individual variables have been declared system wide
- external programs could allocate memory in a non shitty way by using the kernel's malloc functions as long as they remember to clear their memory afterwards
- how to deal with programs allocating memory and then forgetting to free it? idk, current idea include:
    -each program has to keep track of its variable start addresses and part of the program end process involves going back and deleting them all
    -make seperate allocation functions for programs to use that put start and end addresses in a seperate array from the system's
    -each program has it's own copy of the memory.h class and its own dynamic array of memory end pointers. The kernel merely hold a program data start pointer for each running program and deletes its contents once a program stops running

note that correct implementation of everything on this page may eventually enable a wider variety of GNU code to actually work such as math and string
*/

/*allocates memory in a dynamic way
does not write zeros to allocated memory
size = size of memory to allocate in bytes
returned pointer is the start (lower bounds) of the allocated memory location
*/
void *malloc(unsigned int size);

/*allocates memory in a dynamic way
writes zeros to allocated memory
size = size of memory to allocate in bytes
returned pointer is the start (lower bounds) of the allocated memory location
*/
void *calloc(unsigned int num_items, unsigned int size, unsigned int clearingOffset);   //default arguments for clearingoffset cause an error. gcc doesn't like it and it will fight you in unpredictable ways when you try to redefine calloc or any of ther other gnu memory management functions
//void *calloc(unsigned int num_items, unsigned int size);

/*reallocates memory to a new address
ptr = desired lower bounds of address to move block of data to
size = size of memory to reallocate in bytes
returned pointer is the start (lower bounds) of the allocated memory location. null if operation was unsuccessful
*/
void *realloc(void *ptr, unsigned int size);

/*
deallocates a block of memory
ptr=address of memory block to free
*/
void free(void *ptr);

//copy memory from 1 location in ram to another location in ram
void * memcpy ( void * destination, const void * source, unsigned int num );

//does the same thing as memcpy. It trades speed for the advantage of being possibly less buggy and more reliable
void *slow_memcpy(void *dst, const void *src, unsigned int len);

//helper functions that go with the allocation functions for better source code organization

//figures out if the first address block and the second address block conflict with each other. If yes, return true. If no, return false
bool conflicts(unsigned int sadr1, unsigned int eadr1, unsigned int sadr2, unsigned int eadr2);

//returns true if the provided start and end address block conflicts with anything in the memory allocation table
bool anyconflict(unsigned int start, unsigned int end);

/*
start = start address in memory(input).
*position = position in memf/memb memory table (output, optional. Set to nullptr if index position isn't needed)
return type = size in bytes of the memory block
returns 0 if no match found
*/
unsigned int getSizeOfMemBlock(void* start, unsigned int *position);

/*void *operator new(size_t size)
{
    return malloc(size);
}
 
void *operator new[](size_t size)
{
    return malloc(size);
}

void* operator new[](size_t size, void* p)
{ 
    //return p;
    return calloc(1,size);
}

void* operator new(size_t size, void* p)
{ 
    return calloc(1,size); 
}

void* operator new(size_t size, char* p)
{ 
    return calloc(1,size); 
}
 
void operator delete(void *p)
{
    free(p);
}
 
void operator delete[](void *p)
{
    free(p);
}*/
 

#include "memory.cpp"

#endif