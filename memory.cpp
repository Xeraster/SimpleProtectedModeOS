/*allocates memory in a dynamic way
does not write zeros to allocated memory
size = size of memory to allocate in bytes
returned pointer is the start (lower bounds) of the allocated memory location
*/
void *malloc(unsigned int size)
{
    //lol jk it actually does write zeros to newly allocated memory
    return calloc(1, size);
}

void *alloca(unsigned int size)
{
    //lol jk it actually does write zeros to newly allocated memory
    return calloc(1, size);
}

/*allocates memory in a dynamic way
writes zeros to allocated memory
size = size of memory to allocate in bytes
returned pointer is the start (lower bounds) of the allocated memory location
*/
void* calloc(unsigned int num_items, unsigned int size)
{
    void* pointer;
    unsigned int start = 0;
    unsigned int end = 0;

    *(char*)0xB805A = 'c';
	*(char*)0xB805B = 0x0C;

    //if memory table is completely empty, nothing can conflict with it. Place at beginning of table and call it a day
    //changed to a nonconst to see if that makes it not crash on non-emulator systems but it doesn't, at least not here
    if (memf.getSize_noconst() == 0)
    {
        start = programHeapStart;
        end = programHeapStart + (size*num_items);
        *(char*)0xB805C = 'c';
        *(char*)0xB805D = 0x0C;

    }
    else
    {
        //set preferred location
        //start = memb.back() + 1;
        //end = start + (size*num_items);

        start = programHeapStart;
        end = programHeapStart + (size*num_items);
        //probably the most common case
        /*if (conflicts(memf.back(), memb.back(), start, end))
        {

            bool stillConflict = true;
            int i = 0;
            while (stillConflict && i < totalMemory)
            {
                start = programHeapStart + i;
                end = start + (size*num_items);
                stillConflict = anyconflict(start, end);
                i++;
            }
            
        }*/
        *(char*)0xB805E = 'c';
        *(char*)0xB805F = 0x0C;
        //if it doesn't conflict, make sure it doesn't conflict with anything else
        if (anyconflict(start, end))
        {

            //if it conflicts, find a new spot
            bool stillConflict = true;
            int i = 0;
            while (stillConflict && i < totalMemory)
            {
                start = programHeapStart + i;
                end = start + (size*num_items);
                stillConflict = anyconflict(start, end);
                i++;
            }
        }

        *(char*)0xB8060 = 'c';
        *(char*)0xB8061 = 0x0C;
            //no conflict? great. Continue with the previously calculated values

    }

    *(char*)0xB8062 = 'c';
    *(char*)0xB8063 = 0x0C;

    //write zeros to all the stuff in the memory block
    for(int m = 0; m < (size*(num_items-1)); m++)
    //for(int m = 0; m < end - start; m++)
    {
        //commenting this out STILL solves bugs which isn't good. Something is wrong but I still haven't ben able to figure out what. 
        //06/02/23 I got those aformentioned bugs solved a long time ago
        //this needs to be done 1 byte/8 bits at a time or else strange things will happen
        *(char*)(start+m) = char(0);

        //oof, glad I caught this bug before things started getting too complicated. We don't erase stuff beyond end. Only start. start-end needs to be cleared
        //*(unsigned int*)(end+m) = 0;

        //fuck it. print the addresses that its trying to erase
        //if (start == commandBufferAddress)
        //{
        //    print32bitNumber(start+m,80*(m)+40);
        //}
    }

    *(char*)0xB8064 = 'c';
    *(char*)0xB8065 = 0x0C;

    //put the addresses into the memory allocation tables
    memf.push_back(start, false);
    memb.push_back(end, false);

    *(char*)0xB8066 = 'c';
    *(char*)0xB8067 = 0x0C;

    //assign the start address to the pointer that's going to get returned
    pointer = (void*)start;

    *(char*)0xB8068 = 'c';
    *(char*)0xB8069 = 0x0C;

    return pointer;
}

/*reallocates memory to a new address
ptr = desired lower bounds of address to move block of data to
size = size of memory to reallocate in bytes
returned pointer is the start (lower bounds) of the allocated memory location. null if operation was unsuccessful
*/
void *realloc(void *ptr, unsigned int size)
{
    //__asm__("xchg bx, bx");//trying to get a breakpoint in bochs. xchg bx, bx doesnt work. int 3 doesnt work. the button doesnt work. I cant use the breakpoint library.
    //breakpointHack();//fine then, i'll just use this instead
    *(char *)0xB804E = 'r';
    *(char*)0xB804F = 0x0A;

    //if pointer is the same as memf or memb, do different stuff than if it isn't
    if (ptr == (void*)&memf || ptr == (void*)&memb)
    {
        //maybe if we expand them both at the same time, the system won't crash

        //save the original positions of the arrays for later
        unsigned int oaadrf = (unsigned int)memf.getAddress();
        unsigned int oaadrb = (unsigned int)memb.getAddress();

        //save the current positions of both the memory allocation tables
        void *pf = (void*)&memf;
        void *pb = (void*)&memb;
        unsigned int newSize = memf.max_size() + 10;

        //manually allocate memory for 2 new, bigger memory address arrays
        unsigned int *npf = (unsigned int*)calloc(newSize, sizeof(unsigned int));
        unsigned int *npb = (unsigned int*)calloc(newSize, sizeof(unsigned int));

        //we now have the location of the new memory address arrays. Now, copy the data from the old ones to the new ones
        for (int i = 0; i < (memf.getSize()); i++)
        {
            npf[i] = memf.at(i);
            npb[i] = memb.at(i);
        }

        //the new data has been copied.

        memf.manual_resize(newSize, npf);
        memb.manual_resize(newSize, npb);

        //now free the memory that the old entries used up
        unsigned int findex = 0;
        if (memf.find(oaadrf, &findex))
        {
            memf.erase(findex);
            memb.erase(findex);
        }

        unsigned int bindex = 0;
        if (memf.find(oaadrb, &bindex))
        {
            memf.erase(bindex);
            memb.erase(bindex);
        }

        //return nullptr;
        return 0;
    }
    *(char*)0xB8050 = 'r';
	*(char*)0xB8051 = 0x0A;

    //now that this is done, copy the memory from the old location to the new location
    //breakpointHack();
    unsigned int pos = 0; // changed from unsigned int pos; 05/30/22
    unsigned int oldSize = getSizeOfMemBlock(ptr, &pos);
    //breakpointHack();
    *(char*)0xB8052 = 'r';
	*(char*)0xB8053 = 0x0A;

    ///first, allocate new memory block of the correct size
    void* pointer = calloc(1, size);
    *(char*)0xB8054 = 'r';
	*(char*)0xB8055 = 0x0A;
    //breakpointHack();

    //if old size is larger than new size, this is a shrink resize operation and copying the original size worth of data will cause wierd bugs
    if (oldSize > size)
    {
        *(char*)0xB8056 = 'r';
	    *(char*)0xB8057 = 0x0D;
        memcpy(pointer, ptr, size);
    }
    else
    {
        *(char*)0xB8056 = 'r';
	    *(char*)0xB8057 = 0x0D;
        memcpy(pointer, ptr, oldSize);
    }
    *(char*)0xB8058 = 'r';
	*(char*)0xB8059 = 0x0A;

    return pointer;
}

/*
deallocates a block of memory
ptr=address of memory block to free
*/
void free(void *ptr)
{
    unsigned int index = 0;
    if (memf.find((unsigned int)ptr, &index))
    {
        memf.erase(index);
        memb.erase(index);
    }

    return;
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

//use this instead if there's ever a suspicion that memcpy causes crashes
void *slow_memcpy(void *dst, const void *src, unsigned int len)
{
    char *d = (char*)dst;
    char *s = (char*)src;
    for (int i = 0; i < len; i++)
    {
        d[i] = s[i];
    }

    return dst;
}

bool conflicts(unsigned int sadr1, unsigned int eadr1, unsigned int sadr2, unsigned int eadr2)
{
    //if any of the addresses are zero, it's invalid so just return false
    if (sadr1 == 0 || sadr2 == 0 || eadr1 == 0 || eadr2 == 0) return false;
    //if start address 2 is greater than end address 1, a conflict is impossible
    else if (sadr2 > eadr1) return false;

    //if end address 2 is less than start address 1, a conflict is impossible
    else if (eadr2 < sadr1) return false;

    //if start2 is anywhere between start1 and end1, its a conflict
    else if (sadr2 >= sadr1 && sadr2 <= eadr1) return true;

    //if end2 is anywhere between start1 and end1, it's a conflict
    else if (eadr2 >= sadr1 && eadr2 <= eadr1) return true;

    //if start1 is anywhere between start 2 and end 2, it's a conflict
    else if (sadr1 >= sadr2 && sadr1 <= eadr2) return true;

    //if end1 is anywhere betwen start2 and end2, it's a conflict
    else if (eadr1 >= sadr2 && eadr1 <= eadr2) return true;

    //and lastly, if start2 is less than start1 but end2 is greater than end1, it's a conflict
    else if(eadr2 > eadr1 && sadr2 < sadr1) return true;

    //for fuck sake
    else if (sadr1 == sadr2) return true;
    else if (eadr1 == eadr2) return true;

    else
    {
        return false;
    }


}

//returns true if the provided start and end address block conflicts with anything in the memory allocation table
bool anyconflict(unsigned int start, unsigned int end)
{
    bool conflictFound = false;
    int i = 0;
    while (!conflictFound && i < memf.getSize())
    {
        conflictFound = conflicts(memf.at(i), memb.at(i), start, end);
        i++;
    }

    return conflictFound;
}

unsigned int getSizeOfMemBlock(void* start, unsigned int* position)
{
    bool foundMatch = false;
    *(char*)0xB806A = '$';
    *(char*)0xB806B = 0x0C;
    //fins something in the memory front table that has the same start address as the "start"
    //breakpointHack();
    /**(char *)0xB8070 = (memf.getSize());
    *(char*)0xB8071 = 0x0E;
    *(char*)0xB806C = '*';
    *(char*)0xB806D = 0x0C;
    *(char*)0xB8072 = (memb.getSize());
	*(char*)0xB8073 = 0x0E;*/
    //breakpointHack();
    unsigned int i = 0;
    while (!foundMatch && i < memf.count)
    //while (!foundMatch && i < memf.getSize())
    {
        *(char*)0xB806C = 'm';
        *(char*)0xB806D = 0x09;
        if (memf.at_noconst(i) == (unsigned int)start)
        {
            *(char*)0xB806C = 'm';
            *(char*)0xB806D = 0x0A;
            //if found a matching address, done with loop
            foundMatch = true;
        }
        else
        {
            *(char*)0xB806C = 'm';
            *(char*)0xB806D = 0x0B;
            i++;
        }
    }
    *(char*)0xB806C = '*';
    *(char*)0xB806D = 0x0C;
    //breakpointHack();

    *(char*)0xB806C = 'm';
    *(char*)0xB806D = 0x0C;
    //breakpointHack();

    //set the optional position output to the index position of the found address
    //breakpointHack();
    //*position = (unsigned int)i;  //not nessecary?
    //breakpointHack();
    *(char *)0xB806C = 'm';
    *(char*)0xB806D = 0x0A;

    //if no match found, return 0
    //otherwise, return the size of the found memory block
    if (!foundMatch)
    {
        *(char*)0xB806E = 'm';
        *(char*)0xB806F = 0x0B;
        //breakpointHack();
        return 0;
    }
    else
    {
        *(char*)0xB806E = 'm';
        *(char*)0xB806F = 0x0C;
        //breakpointHack();
        return memb.at_noconst(i) - memf.at_noconst(i);
    }
}