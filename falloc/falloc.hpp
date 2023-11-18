//Fucking Ultimate Commented Casual Allocator
//allocates memory
//no outside libraries. at all

//to make all the complicated functions less complicated, a startup sequence must be run to initialize the memory manager
void initialize()
{
    //create new array at location
    mmt = (memData*)MEMORY_LOC;

    memData newData;
    newData.m_start = 0x20000;
    newData.m_end = 0x20000 + START_TABLE_SIZE;
    
    //fill the newly created memory metadata table with zeros
    memFill(mmt, 0 ,START_TABLE_SIZE);

    mmt[0] = newData;
    currentSize = 1;
    //mainIndex = 0;
}

void *malloc(unsigned int size)
{
    //first, find an unused area of memory that is at least the correct size

    //unsigned int newstart = MEMORY_LOC;
    unsigned int newstart = MEMORY_LOC;
    unsigned int newend = newstart + size;

    //if it doesn't conflict, make sure it doesn't conflict with anything else
    if (anyconflict(newstart, newend))
    {
        //asmOutb('p', 0xE9);
        //if it conflicts, find a new spot
        bool stillConflict = true;
        int i = MEMORY_LOC;
        while (stillConflict && i < MEMORY_LOC + 0xFFFFF)
        {
            //asmOutb('o', 0xE9);
            newstart = i;
            newend = newstart + size;
            stillConflict = anyconflict(newstart, newend);
            i++;
        }
        //intToE9(i, true);
    }

    //make a memData object with the new characteristics
    memData newObject;
    newObject.m_start = newstart;
    newObject.m_end = newend;
    //asmOutb('p', 0xE9);
    void *pppPtr = addItem(newObject);//returns nullptr if allocation failed
    //intToE9((unsigned int)pppPtr);
    //asmOutb('\n', 0xE9);

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
        asmOutb('A', 0xE9);
        memcpy(newPtr, ptr, size);
        asmOutb('J', 0xE9);
    }
    else if (oldSize == 0 || oldSize == -1)
    {
        /*do nothing. 
        
        As I learned in my other 2 attempts at making a memory manager, you do NOT want to pre-clear bytes
        unless you're 100% sure that's the correct move. This is not one of those times. Care about cybersecurity? Don't write leaky programs*/
        asmOutb('B', 0xE9);
    }
    else
    {
        asmOutb('C', 0xE9);
        memcpy(newPtr, ptr, oldSize);
        //we *should* be copying by the same amount in the old size, right?
    }
    asmOutb('K', 0xE9);

    //how we have to free the old one
    //I know, I know, pointers have done enough damage to society already
    if (newPtr != nullptr) free(ptr);
    asmOutb('D', 0xE9);

    return newPtr;
}

void *calloc(unsigned int num, unsigned int size)
{
    void *ptr = malloc(num * size);
    memFill(ptr, 0, num * size);//it doesn't matter how much fucking time I spend on it or how many times i rewrite memory allocation. clearing bytes when allocating always fucks up
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

    unsigned int newstart = MEMORY_LOC;
    unsigned int newend = newstart + newSize;

    //if it doesn't conflict, make sure it doesn't conflict with anything else
    if (anyconflict(newstart, newend))
    {
        //asmOutb('p', 0xE9);
        //if it conflicts, find a new spot
        bool stillConflict = true;
        int i = MEMORY_LOC;
        while (stillConflict && i < MEMORY_LOC + 0XFFFFF)
        {
            //asmOutb('o', 0xE9);
            newstart = i;
            newend = newstart + newSize;
            stillConflict = anyconflict(newstart, newend);
            i++;
        }
        //intToE9(i, true);
    }

    //make a memData object with the new characteristics
    memData newObject;
    newObject.m_start = newstart;
    newObject.m_end = newend;
    //asmOutb('p', 0xE9);
    //void *pppPtr = addItem(newObject);//returns nullptr if allocation failed
    mmt[currentSize] = newObject;
    void* pppPtr = (void*)mmt[currentSize].m_start;
    currentSize++;
    asmOutb('V',0xE9);

    //intToE9((unsigned int)pppPtr);
    asmOutb('\n', 0xE9);

    if (oldSize > newSize)
    {
        asmOutb('A', 0xE9);
        memcpy(pppPtr, mmt, newSize);
        asmOutb('J', 0xE9);
    }
    else if (oldSize == 0 || oldSize == -1)
    {
        /*do nothing. 
        
        As I learned in my other 2 attempts at making a memory manager, you do NOT want to pre-clear bytes
        unless you're 100% sure that's the correct move. This is not one of those times. Care about cybersecurity? Don't write leaky programs*/
        asmOutb('B', 0xE9);
    }
    else
    {
        asmOutb('C', 0xE9);
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

}

//remove an item from memory metadata array at provided position
void removeItem(unsigned int index)
{
    //shift them all down

    //if not the very top element, shift things down
    //if (index != currentSize -1)
    //{
        for (int i = index; i < currentSize; i++)
        {
            //don't try to shift element at position 0 down to position -1
            //if (i != 0)
            //{
                mmt[i] = mmt[i+1];
            //}
        }

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
    memFill(&mmt[currentSize], 0, getSizeOfPointer(mmt));

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
        asmOutb('\n',0xE9);
        asmOutb('r',0xE9);
        asmOutb('e',0xE9);
        //not enough size. resize the memory allocator
        //set the new size to current max size in bytes + the starting size in bytes
        unsigned int newSize = START_TABLE_SIZE + (getMaxSizeMMT());
        asmOutb('e',0xE9);
        intToE9(newSize, false);
        asmOutb('e',0xE9);
        resizeArray(newSize);
        asmOutb('!',0xE9);

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
    //damn troubleshooting memory allocation sucks fuckin' ass
    unsigned int size1 = eadr1 - sadr1;
    unsigned int size2 = eadr2 - sadr2;

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

    //if start2 is anywhere between start 1 and end 1, it's a conflict
    else if (sadr2 >= sadr1 && sadr2 <= eadr1) return true;

    //if end2 is anywhere betwen start1 and end1, it's a conflict
    else if (eadr2 >= sadr1 && eadr2 <= eadr1) return true;

    //and lastly, if start2 is less than start1 but end2 is greater than end1, it's a conflict
    else if(eadr2 > eadr1 && sadr2 < sadr1) return true;

    //and lastly, if start1 is less than start2 but end1 is greater than end2, it's a conflict
    else if ((eadr1 > eadr2 && sadr1 < sadr2)) return true;

    //still doesnt fucking fix that stupid fucking bug wtf
    //else if (sadr1 + size1 >= sadr2) return true;
    //else if (sadr2 + size2 >= sadr1) return true;

    // for fuck sake
    else if (sadr1 == sadr2) return true;
    else if (eadr1 == eadr2) return true;
    else if (sadr1 == eadr2) return true;
    else if (eadr1 == sadr2) return true;

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
    unsigned int sizeOfMem = currentSize;
    while (!conflictFound && w < sizeOfMem)
    {
        conflictFound = conflicts(mmt[w].m_start, mmt[w].m_end, start, end);
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