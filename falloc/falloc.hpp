//Free Ultimate Commented Casual Allocator
//allocates memory
//no outside libraries. at all

//to make all the complicated functions less complicated, a startup sequence must be run to initialize the memory manager
void initialize()
{
    //create new array at location
    mmt = (memData*)MEMORY_LOC;

    memData newData;
    //newData.m_start = 0x20000;
    //newData.m_end = 0x20000 + START_TABLE_SIZE;
    //changed 05/18/2024 to try to make it possible to change the location of the memory map (at compile time, of course)
    newData.m_start = MEMORY_LOC;
    newData.m_end = MEMORY_LOC + START_TABLE_SIZE;
    
    //fill the newly created memory metadata table with zeros
    memFill(mmt, 0 ,START_TABLE_SIZE);

    mmt[0] = newData;
    currentSize = 1;
    //mainIndex = 0;
}

void *malloc(unsigned int size)
{
    *(char*)0xB8050 = 'c';
	*(char*)0xB8051 = 0x06;
    //first, find an unused area of memory that is at least the correct size

    //unsigned int newstart = MEMORY_LOC;
    /*unsigned int newstart = MEMORY_LOC;
    unsigned int newend = newstart + size + 1;

    //if it doesn't conflict, make sure it doesn't conflict with anything else
    *(char*)0xB8052 = 'a';
	*(char*)0xB8053 = 0x0C;
    if (anyconflict(newstart, newend))
    {
        //asmOutb('p', 0xE9);
        //if it conflicts, find a new spot
        bool stillConflict = true;
        unsigned int i = MEMORY_LOC;
        *(char*)0xB8052 = 'W';
	    *(char*)0xB8053 = 0x0A;
        while (stillConflict && i < MEMORY_LOC + 0xFFFFFF)//remove 1 F if you encounter issues
        {
            //asmOutb('o', 0xE9);
            newstart = i;
            newend = newstart + size + 1;
            stillConflict = anyconflict(newstart, newend);
            
            //increasing this by 8 each time seems to be the thing that gives the highest all-around performance. Also tried 4, not as fast as 8. 1 is slower than slow
            //for some reason, increasing by 16 bytes improves performance in a fewer number of different situations and decreases performance in others
            //128 causes things to not work
            //i = 8 causes instability for soem reason
            i+=CONFLICT_CHECK_STEPPING;
            //i+=8;
            //i++;
        }
        //intToE9(i, true);
    }*/
    //unsigned int newstart = (unsigned int)lowestUnusedPositionStartingFromEndOfMemoryAllocation(size, MEMORY_LOC);
    unsigned int newstart = (unsigned int)lowestUnusedPosition(size);
    //unsigned int newstart = (unsigned int)lowestUnusedPositionOld(size); 
    unsigned int newend = newstart + size;
    *(char*)0xB8052 = 0x00;
	*(char*)0xB8053 = 0x00;

    //make a memData object with the new characteristics
    memData newObject;
    newObject.m_start = newstart;
    newObject.m_end = newend;
    //asmOutb('p', 0xE9);
    void *pppPtr = addItem(newObject);//returns nullptr if allocation failed
    //intToE9((unsigned int)pppPtr);
    //asmOutb('\n', 0xE9);
    *(char*)0xB8050 = 0x00;
	*(char*)0xB8051 = 0x00;

    return pppPtr;

}

//basically just malloc with extra steps
void *realloc(void *ptr, unsigned int size)
{   asmOutb('n', 0xE9);
    //first run malloc
    void* newPtr = malloc(size);
    unsigned int oldSize = getSizeOfPointer(ptr);

    if (oldSize > size)
    {
        //asmOutb('A', 0xE9);
        memcpy(newPtr, ptr, size);
        //asmOutb('J', 0xE9);
    }
    else if (oldSize == 0 || oldSize == -1)
    {
        /*do nothing. 
        
        As I learned in my other 2 attempts at making a memory manager, you do NOT want to pre-clear bytes
        unless you're 100% sure that's the correct move. This is not one of those times. Care about cybersecurity? Don't write leaky programs*/
        //asmOutb('B', 0xE9);
    }
    else
    {
        //asmOutb('C', 0xE9);
        memcpy(newPtr, ptr, oldSize);
        //we *should* be copying by the same amount in the old size, right?
    }
    //asmOutb('K', 0xE9);

    //how we have to free the old one
    //I know, I know, pointers have done enough damage to society already
    if (newPtr != nullptr) free(ptr);
    //asmOutb('D', 0xE9);

    return newPtr;
}

void *calloc(unsigned int num, unsigned int size)
{
    void *ptr = malloc(num * size);
    memFill(ptr, 0, num * size);//it doesn't matter how much time I spend on it or how many times i rewrite memory allocation. clearing bytes when allocating always causes issues. 20/28/2025: I can't remember if ALL the memory bugs got fixed or just most of them.
    return ptr;
}

//find the memory entry that corresponds to the pointer and remove it from the array
void free(void *ptr)
{
    //find the array index location of the memdata entry for the given memory allocation
    unsigned int where = getIndexOfPointer(ptr);

    //if -1, nothing to free
    if (where != -1)
    {
        //get it removed from the array
        removeItem(where);   
    }
}

//some programs need this, so it's important that this function works exactly the same way the "real" memset c++ function works
void *memset(void *ptr, int value, unsigned int num)
{
    //I don't know if there is supposed to be handling for if value is more than an 8 bit value
    for(int i = 0; i < num; i++)
    {
        *(char*)(ptr+i) = (char)value;
    }

    return ptr;
}

//stuff besides realloc has to be used for resizing the memData table
void resizeArray(unsigned int newSize)
{
    //allocate a pointer of the new size
    //void* newptr = realloc(mmt,newSize);
    //asmOutb('e',0xE9);
    //void* newptr = malloc(newSize);
    //asmOutb('e',0xE9);
    //unsigned int oldSize = getSizeOfPointer(mmt);
    //asmOutb('e',0xE9);
    //intToE9((unsigned int)newptr, false);
    //asmOutb('e',0xE9);
    unsigned int oldSize = getMaxSizeMMT();//old size in bytes

    //unsigned int newstart = MEMORY_LOC;
    unsigned int newstart = (unsigned int)lowestUnusedPosition(newSize);
    unsigned int newend = newstart + newSize + 1;

    //if it doesn't conflict, make sure it doesn't conflict with anything else
    /*if (anyconflict(newstart, newend))
    {
        //asmOutb('p', 0xE9);
        //if it conflicts, find a new spot
        bool stillConflict = true;
        int i = MEMORY_LOC;
        while (stillConflict && i < MEMORY_LOC + 0xFFFFF)
        {
            //asmOutb('o', 0xE9);
            newstart = i;
            newend = newstart + newSize + 1;
            stillConflict = anyconflict(newstart, newend);
            //i++;    //do it the slow way since this doesn't happen that often
            i+=CONFLICT_CHECK_STEPPING;
            //i+=8;
        }
        //intToE9(i, true);
    }*/



    //make a memData object with the new characteristics
    memData newObject;
    newObject.m_start = newstart;
    newObject.m_end = newend;
    //asmOutb('p', 0xE9);
    //void *pppPtr = addItem(newObject);//returns nullptr if allocation failed
    mmt[currentSize] = newObject;
    void* pppPtr = (void*)mmt[currentSize].m_start;
    currentSize++;
    //asmOutb('V',0xE9);

    //intToE9((unsigned int)pppPtr);
    //asmOutb('\n', 0xE9);

    if (oldSize > newSize)
    {
        //asmOutb('A', 0xE9);
        memcpy(pppPtr, mmt, newSize);
        //asmOutb('J', 0xE9);
    }
    else if (oldSize == 0 || oldSize == -1)
    {
        /*do nothing. 
        
        As I learned in my other 2 attempts at making a memory manager, you do NOT want to pre-clear bytes
        unless you're 100% sure that's the correct move. This is not one of those times. Care about cybersecurity? Don't write leaky programs*/
        //asmOutb('B', 0xE9);
    }
    else
    {
        //asmOutb('C', 0xE9);
        memcpy(pppPtr, mmt, oldSize);
        //we *should* be copying by the same amount in the old size, right?
    }

    //now that all the data has been copied to the correct address, 
    //free(mmt);
    unsigned int oldPtr = getIndexOfPointer((void*)mmt);
    mmt = (memData*)pppPtr;

    //find the pointer for the old one and get rid of it
    getIndexOfPointer((void*)oldPtr);
    removeItem(oldPtr);     //remove it
}

//returns the first position in memory that is unallocated and of the correct size
void *lowestUnusedPosition(unsigned int size)
{
    //unsigned int sizeToUse = currentSize - 2;
    //unsigned int newstart = MEMORY_LOC;
    unsigned int newstart = mmt[0].m_end;
    unsigned int newend = newstart + size;
    if (anyconflict(newstart, newend))
    {
        //asmOutb('p', 0xE9);
        //if it conflicts, find a new spot
        bool stillConflict = true;
        //unsigned int i = MEMORY_LOC;
        unsigned int i = 1;
        *(char*)0xB8052 = 'W';
	    *(char*)0xB8053 = 0x0A;
        //check the boundaries BETWEEN blocks instead of every byte in existence
        unsigned int highestEndSoFar = 0;
        while (stillConflict && i < currentSize-2 && currentSize > 10/*i < MEMORY_LOC + 0xFFFFFF*/)//remove 1 F if you encounter issues
        {
            //asmOutb('o', 0xE9);
            newstart = mmt[i].m_end;
            newend = newstart + size;
            stillConflict = anyconflict(newstart, newend);

            if ((mmt[i].m_end) > highestEndSoFar)
            {
                highestEndSoFar = mmt[i].m_end;
            }
            
            //increasing this by 8 each time seems to be the thing that gives the highest all-around performance. Also tried 4, not as fast as 8. 1 is slow
            //for some reason, increasing by 16 bytes improves performance in a fewer number of different situations and decreases performance in others
            //128 causes things to not work
            //i = 8 causes instability for soem reason
            //i+=CONFLICT_CHECK_STEPPING;
            i++;
            //i+=8;
            //i++;
        }

        //if the while loop was unable to find an empty in-between spot, put this block at the end of the very last block
        if (stillConflict)
        {
            *(char*)0xB8052 = 'S';
	        *(char*)0xB8053 = 0x0A;
            //newstart = mmt[currentSize].m_end + 1;
            newstart = (unsigned int)lowestUnusedPositionStartingFromEndOfMemoryAllocation(size, highestEndSoFar);
        }
        //intToE9(i, true);
    }

    return (void*)newstart;
}

//The same as the "old" inefficient way except that it starts at the last memory block and THEN checks every byte in memory. Hopefully this will improve speed.
//Confirmed working.
void *lowestUnusedPositionStartingFromEndOfMemoryAllocation(unsigned int size, unsigned int startAddress)
{
    //unsigned int newstart = MEMORY_LOC;
    unsigned int newstart = startAddress;     //check every byte, but start at the end of the memory table
    unsigned int newend = newstart + size + 1;
    if (anyconflict(newstart, newend))
    {
        //asmOutb('p', 0xE9);
        //if it conflicts, find a new spot
        bool stillConflict = true;
        unsigned int i = startAddress + 1;
        *(char*)0xB8052 = 'W';
	    *(char*)0xB8053 = 0x0A;
        while (stillConflict && i < MEMORY_LOC + 0xFFFFFF)//remove 1 F if you encounter issues
        {
            //asmOutb('o', 0xE9);
            newstart = i;
            newend = newstart + size + 1;
            stillConflict = anyconflict(newstart, newend);
            
            //increasing this by 8 each time seems to be the thing that gives the highest all-around performance. Also tried 4, not as fast as 8. 1 is slow
            //for some reason, increasing by 16 bytes improves performance in a fewer number of different situations and decreases performance in others
            //128 causes things to not work
            //i = 8 causes instability for soem reason
            //i+=CONFLICT_CHECK_STEPPING;
            //i+=8;
            i++;
        }
        //intToE9(i, true);
    }
    return (void*)newstart;
}

//returns the first position in memory that is unallocated and of the correct size the slow but non-buggy way
void* lowestUnusedPositionOld(unsigned int size)
{
    //unsigned int newstart = MEMORY_LOC;
    unsigned int newstart = MEMORY_LOC;
    unsigned int newend = newstart + size + 1;
    if (anyconflict(newstart, newend))
    {
        //asmOutb('p', 0xE9);
        //if it conflicts, find a new spot
        bool stillConflict = true;
        unsigned int i = MEMORY_LOC;
        *(char*)0xB8052 = 'W';
	    *(char*)0xB8053 = 0x0A;
        while (stillConflict && i < MEMORY_LOC + 0xFFFFFF)//remove 1 F if you encounter issues
        {
            //asmOutb('o', 0xE9);
            newstart = i;
            newend = newstart + size + 1;
            stillConflict = anyconflict(newstart, newend);
            
            //increasing this by 8 each time seems to be the thing that gives the highest all-around performance. Also tried 4, not as fast as 8. 1 is slow
            //for some reason, increasing by 16 bytes improves performance in a fewer number of different situations and decreases performance in others
            //128 causes things to not work
            //i = 8 causes instability for soem reason
            i+=CONFLICT_CHECK_STEPPING;
            //i+=8;
            //i++;
        }
        //intToE9(i, true);
    }
    return (void*)newstart;
}

//remove an item from memory metadata array at provided position
void removeItem(unsigned int index)
{
    //shift them all down

    //if not the very top element, shift things down
    //if (index != currentSize -1)
    //{
        for (int i = index; i < currentSize - 1; i++)     //experimented with changing to currentSize - 1 05/10/24. Inconclusive results.
        {
            //don't try to shift element at position 0 down to position -1
            //if (i != 0)
            //{
                mmt[i] = mmt[i+1];
            //}
        }

        //redo
        //backwards for loop because reasons
        /*for (int i = currentSize; i > index; i--)
        {
            mmt[i-1] = mmt[i];
        }*/

        //if memory metadata table entry is in the part that got scooched down, decrement the pointer
        //if (mainIndex > index)
        //{
        //    mainIndex--;
        //}
    //}

    //remember to decrement the housekeeping variable
    if (currentSize > 0)
    {
        currentSize--;
    }

    //fill the last memory metadata cell. all empty spaces must contain zeros because otherwise debugging it with a memory viewer is impossible
    //memFill(&mmt[currentSize], 0, getSizeOfPointer(mmt));     //backspacing this fixes The C++ Bug ironically. Wow. I still have to fix memory leaks though

}

void* addItem(memData newitem)
{
    void *ptr = nullptr;
    //if there is simply room for one more, add it
    if (currentSize + 2 < getMaxSizeMMT()/sizeof(memData))
    {
        mmt[currentSize] = newitem;
        ptr = (void*)mmt[currentSize].m_start;
        currentSize++;
        //asmOutb('V',0xE9);
    }
    //if there is not enough room for one more, make a new bigger one 
    else
    {
        //oh boy
        //asmOutb('\n',0xE9);
        //asmOutb('r',0xE9);
        //asmOutb('e',0xE9);
        //not enough size. resize the memory allocator
        //set the new size to current max size in bytes + the starting size in bytes
        unsigned int newSize = START_TABLE_SIZE + (getMaxSizeMMT());    //make the new size START_TABLE_SIZE bytes larger
        //asmOutb('e',0xE9);
        //intToE9(newSize, false);
        //asmOutb('e',0xE9);
        resizeArray(newSize);
        //asmOutb('!',0xE9);

        //we need to find a way to return the address of where the new object got reallocated to
        mmt[currentSize] = newitem;

        ptr = (void*)mmt[currentSize].m_start;
        currentSize++;
    }

    return ptr;
}

//returns the size of the memory metadata table
unsigned int getMaxSizeMMT()
{
    memData *theOne = findMMU();
    return theOne->m_end - theOne->m_start;
}

//find and return a memdata object with the same start number as the array
memData *findMMU()
{
    for (int i = 0; i < currentSize; i++)
    {
        if (mmt[i].m_start == (unsigned int)mmt)
        {
            return &mmt[i];
        }
    }
}

void memFill(void *dst, char c, unsigned int len)
{
    char *d = (char*)dst;
    for (int i = 0; i < len; i++)
    {
        d[i] = c;
    }
}

bool conflicts(unsigned int sadr1, unsigned int eadr1, unsigned int sadr2, unsigned int eadr2)
{
    //damn troubleshooting memory allocation is hard
    //unsigned int size1 = eadr1 - sadr1;
    //=unsigned int size2 = eadr2 - sadr2;

    //if any of the addresses are zero, it's invalid so just return false
    if (sadr1 <= eadr2 && sadr2 <= eadr1) return true;
    //else if (sadr1 == 0 || sadr2 == 0 || eadr1 == 0 || eadr2 == 0) return true;//uncommented on 05/27/2024
    else if (sadr1 >= eadr1 || sadr2 >= eadr2) return true;
    //else if (sadr1 >= eadr1) return true;

    //maybe testing for extra cases will make it not mess up when checking for free memory in increments larger than 4 bytes
    /*else if (sadr1 == sadr2) return true;
    else if (eadr1 == eadr2) return true;
    else if (sadr1 == eadr2) return true;
    else if (eadr1 == sadr2) return true;*/

    return false;

    //if start address 2 is greater than end address 1, a conflict is impossible
    //else if (sadr2 > eadr1) return false;

    //if end address 2 is less than start address 1, a conflict is impossible
    //else if (eadr2 < sadr1) return false;

    //return false;

    //if start2 is anywhere between start1 and end1, its a conflict
    if (sadr2 >= sadr1 && sadr2 <= eadr1) return true;

    //if end2 is anywhere between start1 and end1, it's a conflict
    else if (eadr2 >= sadr1 && eadr2 <= eadr1) return true;

    //if start1 is anywhere between start 2 and end 2, it's a conflict
    //else if (sadr1 >= sadr2 && sadr1 <= eadr2) return true;

    //if end1 is anywhere betwen start2 and end2, it's a conflict
    //else if (eadr1 >= sadr2 && eadr1 <= eadr2) return true;

    //COMMENTED OUT ON 02/27/2024
    //If you run into weird memory bugs, this is the first thing that should be uncommented out
    //if start2 is anywhere between start 1 and end 1, it's a conflict
    else if (sadr2 >= sadr1 && sadr2 <= eadr1) return true;

    //if end2 is anywhere betwen start1 and end1, it's a conflict
    else if (eadr2 >= sadr1 && eadr2 <= eadr1) return true;

    //and lastly, if start2 is less than start1 but end2 is greater than end1, it's a conflict
    else if(eadr2 >= eadr1 && sadr2 <= sadr1) return true;

    //and lastly, if start1 is less than start2 but end1 is greater than end2, it's a conflict
    else if ((eadr1 >= eadr2 && sadr1 <= sadr2)) return true;

    //still doesnt fucking fix that stupid fucking bug wtf
    //else if (sadr1 + size1 >= sadr2) return true;
    //else if (sadr2 + size2 >= sadr1) return true;

    else
    {
        return false;
    }


}

//returns true if the provided start and end address block conflicts with anything in the memory allocation table
bool anyconflict(unsigned int start, unsigned int end)
{
    /*bool conflictFound = false;
    int i = 0;
    while (!conflictFound && i < memf.getSize())
    {
        conflictFound = conflicts(memf.at(i), memb.at(i), start, end);
        i++;
    }

    return conflictFound;*/
    bool conflictFound = false;

    int w = 0;
    //unsigned int sizeOfMem = getMaxSizeMMT()/sizeof(memData);
    //unsigned int sizeOfMem = currentSize;
    while (!conflictFound && w < currentSize)
    {
        //maybe checking if the memory entry ahead of it also conflicts or not will allow me to be able to use larger block sizes without issues
        //conflictFound = conflicts(mmt[w].m_start, mmt[w].m_end, start-1, end+1) || conflicts(mmt[w+1].m_start, mmt[w+1].m_end, start-1, end+1);
        conflictFound = conflicts(mmt[w].m_start, mmt[w].m_end, start-1, end+1);//add an extra buffer of bullshit. GET RID OF THE -1 AND +1 (and reduce CONFLICT_CHECK_STEPPING to 4 if any bugs are encountered)
        w++;
    }

    return conflictFound;
}

//if ptr is a memory block, return its size. if not, it will return -1
unsigned int getSizeOfPointer(void *ptr)
{
    for (int i = 0; i < currentSize; i++)
    {
        if (mmt[i].m_start == (unsigned int)ptr)
        {
            return (mmt[i].m_end - mmt[i].m_start);
        }
    }

    return -1;
}

unsigned int getIndexOfPointer(void *ptr)
{
    for (int i = 0; i < currentSize; i++)
    {
        if (mmt[i].m_start == (unsigned int)ptr)
        {
            //intToE9(mmt[i].m_start, false);
            return i;
        }
    }

    return -1;
}

//returns total number of bytes of allocated memory.
unsigned int totalUsedMemory()
{
    //bytes counted so far
    unsigned int numSoFar = 0;

    //loop through the entire memData array and just add up all the numbers
    for (int i = 0; i < currentSize; i++)
    {
        //save pointer to the mem metadata entry
        memData *ptr = &mmt[i];

        numSoFar += ptr[i].m_end - ptr[i].m_start;
    }

    //there. all the amounts should be added up now
    return numSoFar;
}

//returns true if there is a memory allocation entry that starts at the specified address. returns false if otherwise
bool allocationExists(void *ptr)
{
    //"loop until it finds the thing" cookie cutter code
    bool foundMatch = false;
    int i = 0;
    while (!foundMatch && i < currentSize)
    {
        if (mmt[i].m_start == (unsigned int)ptr)
        {
            foundMatch = true;
        }
        i++;
    }

    return foundMatch;
}