/*allocates memory in a dynamic way
does not write zeros to allocated memory
size = size of memory to allocate in bytes
returned pointer is the start (lower bounds) of the allocated memory location
*/
void *malloc(size_t size)
{
    //lol
    return calloc(1, size);
}

void *alloca(size_t size)
{
    //lol
    return calloc(1, size);
}

/*allocates memory in a dynamic way
writes zeros to allocated memory
size = size of memory to allocate in bytes
returned pointer is the start (lower bounds) of the allocated memory location
*/
void* calloc(size_t num_items, size_t size)
{
    void* pointer;
    unsigned int start = 0;
    unsigned int end = 0;

    //if memory table is completely empty, nothing can conflict with it. Place at beginning of table and call it a day
    if (memf.getSize() == 0)
    {
        start = programHeapStart;
        end = programHeapStart + (size*num_items);

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
            //no conflict? great. Continue with the previously calculated values

    }

    //write zeros to all the stuff in the memory block
    for(int m = 0; m < (size*num_items); m++)
    {
        *(unsigned int*)(start+m) = 0;
        *(unsigned int*)(end+m) = 0;
    }

    //put the addresses into the memory allocation tables
    memf.push_back(start, false);
    memb.push_back(end, false);

    //assign the start address to the pointer that's going to get returned
    pointer = (void*)start;

    return pointer;
}

/*reallocates memory to a new address
ptr = desired lower bounds of address to move block of data to
size = size of memory to reallocate in bytes
returned pointer is the start (lower bounds) of the allocated memory location. null if operation was unsuccessful
*/
void *realloc(void *ptr, size_t size)
{

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
        for (int i = 0; i < memf.getSize(); i++)
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

        return nullptr;

    }
    ///first, allocate new memory block of the correct size
    void* pointer = calloc(1, size);

    //now that this is done, copy the memory from the old location to the new location
    unsigned int pos;
    unsigned int oldSize = getSizeOfMemBlock(ptr, &pos);
    memcpy(pointer, ptr, oldSize);

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
void *memcpy(void *dst, const void *src, size_t len)
 {
         size_t i;
 
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

bool conflicts(unsigned int sadr1, unsigned int eadr1, unsigned int sadr2, unsigned int eadr2)
{
    //if any of the addresses are zero, it's invalid so just return false
    if (sadr1 == 0 || sadr2 == 0 || eadr1 == 0 || eadr2 == 0) return false;
    //if start address 2 is greater than end address 1, a conflict is impossible
    else if (sadr2 > eadr1) return false;

    //if end address 2 is less than start address 1, a conflict is impossible
    else if (eadr2 < sadr1) return false;

    //if start2 is anywhere between start1 and end1, its a conflict
    else if (sadr2 >=  sadr1 && sadr2 <= eadr1) return true;

    //if end2 is anywhere between start1 and end1, it's a conflict
    else if (eadr2 >=  sadr1 && eadr2 <= eadr1) return true;

    //and lastly, if start2 is less than start1 but end2 is greater than end1, it's a conflict
    else if(eadr2 > eadr1 && sadr2 < sadr1) return true;

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
    int i = 0;
    while (!foundMatch && i < memf.getSize())
    {
        if (memf.at(i) == (unsigned int)start)
        {
            foundMatch = true;
        }
        i++;
    }

    *position = i;

    if (!foundMatch) return 0;
    else
    {
        return memb.at(i) - memf.at(i);
    }
}