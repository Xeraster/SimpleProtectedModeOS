template <typename T>
rarray<T> :: rarray()
{   
    *(char*)0xB8012 = 'C';
	*(char*)0xB8013 = 0x0B;
    array = (T*)calloc(10, sizeof(T));
    *(char*)0xB8014 = 'V';
	*(char*)0xB8015 = 0x0B;
    count = 0;
    maxSize = 10;
    m_isMemoryTable = false;
    m_init = true;
    return;
}

/*template <typename T>
dynarray<T> :: ~dynarray()
{
    free(array);
    return;
}*/

//used by realloc when attempting to realloc the memory allocation table dynarrays
template <typename T>
void rarray<T> :: manual_resize(unsigned int newMaxSize, void* newArrayAddress)
{
    array = (T*)newArrayAddress;
    maxSize = newMaxSize;
}

template <typename T>
void rarray<T> :: push_back(T item)
{
    *(char*)0xB803A = '!';
	*(char*)0xB803B = 0x0E;
    if (!m_init)
    {
        *(char*)0xB803A = '3';
	    *(char*)0xB803B = 0x0A;
        array = (T*)calloc(10, sizeof(T));
        *(char*)0xB803A = '!';
	    *(char*)0xB803B = 0x0D;
        count = 0;
        maxSize = 10;
        m_isMemoryTable = false;
        m_init = true;

        //then don't forget to finish adding the pending data
        array[count] = item;
        count++;
        *(char*)0xB803A = 'B';
	    *(char*)0xB803B = 0x0D;

    }
    else if (count < maxSize)
    {
        //count++;
        array[count] = item;
        count++;
        *(char*)0xB803A = 'B';
	    *(char*)0xB803B = 0x0E;
    }
    else
    {
        *(char*)0xB803A = 'H';
	    *(char*)0xB803B = 0x0E;
        resize(maxSize + 10);
        *(char*)0xB803A = 'H';
	    *(char*)0xB803B = 0x0D;
        array[count] = item;
        count++;
        *(char*)0xB803A = 'B';
	    *(char*)0xB803B = 0x0F;
    }
    *(char*)0xB803A = '3';
	*(char*)0xB803B = 0x0E;

    //return;
}

//delete the last element
template <typename T>
void rarray<T> :: pop_back()
{
    if (count > 0)
    {
        count--;
    }
}

//inserts an item at designated position
template <typename T>
void rarray<T> :: insert(unsigned int position, T item)
{
    //if count is close to the max size, grow it a little before doing this
    if (maxSize <= count + 1)
    {
        resize(maxSize+10);
    }

    //once that's over with, do the insertion
    count++;
    for (int i = count-2; i >= position; i--)
    {
        array[i+1] = array[i];
    }

    //now that everything has been shifted up, assign the item to the desired position
    array[position] = item;

    return;
}

//deletes element at position and then shifts everything down
template <typename T>
void rarray<T> :: erase(unsigned int position)
{
    for (int i = position; i < count - 1; i++)
    {
        array[i] = array[i+1];
    }
    count--;

    return;
}

//deletes everything in the data object
template <typename T>
void rarray<T> :: clear()
{
    //I mean, right? Am I missing something or is this all there is to it?
    //count = 0;
    //free(array);
    //m_init = false;
    //maxSize = 0;
    *(char*)0xB8040 = 'L';
	*(char*)0xB8041 = 0x0C;
    //resize(10);

    //for (int i = 0; i < maxSize; i++)
    //{
    //    array[i] = 0;
   // }

    //count = 0;
    //move to a different fucking spot
    //unsigned int newSizeInBytes = sizeof(T)*maxSize;
    //void* oldPtr = (void*)array;
    //array = (T*)realloc(oldPtr, newSizeInBytes);
    //if (oldPtr != array) free(oldPtr);
    count = 0;
    maxSize = 0;
    //m_init = false;
    free(array);
    // shrink_to_fit();
    // maxSize = 10;
    // free(array);
    //resize(10);

    return;
}

//makes the data object become the same size as newSize
template <typename T>
void rarray<T> :: resize(unsigned int newSize)
{
    //clear previous output
    *(char*)0xB8043 = 0x00;
    *(char*)0xB8045 = 0x00;
    *(char*)0xB8047 = 0x00;
    *(char*)0xB8043 = 0x00;
    *(char*)0xB8049 = 0x00;
    *(char*)0xB804B = 0x00;
    *(char*)0xB804D = 0x00;

    unsigned int oldSize = maxSize*sizeof(T);
    maxSize = newSize;
    *(char*)0xB8042 = 'R';
	*(char*)0xB8043 = 0x0D;
    unsigned int newSizeInBytes = sizeof(T)*newSize;
    *(char*)0xB8044 = 'R';
	*(char*)0xB8045 = 0x0D;
    void* oldPtr = (void*)array;
    *(char*)0xB8046 = 'R';
	*(char*)0xB8047 = 0x0D;
    //array = (T*)realloc(array, newSizeInBytes);
    //array = (T*)malloc(newSizeInBytes);
    array = (T*)calloc(1, newSizeInBytes);
    if (oldSize < newSizeInBytes)
    {
        slow_memcpy(array, oldPtr, oldSize);
    }
    else
    {
        slow_memcpy(array, oldPtr, newSizeInBytes);
    }
    *(char*)0xB8048 = 'R';
	*(char*)0xB8049 = 0x0D;
    
    //don't forget to delete the old array
    //theoretically, this oldPtr should never be equal to this array. I however can't seem to prevent it from happening so I might as well define the correct behavior for when it does happen
    if (oldPtr != array) free(oldPtr);
    *(char*)0xB804A = 'R';
	*(char*)0xB804B = 0x0D;

    //adjust count. 
    if (count > maxSize && maxSize > 0)
    {
        count = maxSize - 1;
    }
    *(char*)0xB804C = 'R';
	*(char*)0xB804D = 0x0E;

    //return;
}

//changes the size of the data object to exactly contain everything and no more
template <typename T>
void rarray<T> :: shrink_to_fit()
{
    //it needs a buffer of like 5 or it won't work for some reason
    resize(count+5);
}

//returns whatever element is at front
template <typename T>
T& rarray<T> :: front()
{
    return array[0];
}

//returns whatever element is at back
template <typename T>
T& rarray<T> :: back()
{
    if (count > 0) return array[count-1];
    else return *(T*)nullptr;//put zero here if compiler complains about nullptr
}

template <typename T>
bool rarray<T> :: find(T thingToFind, unsigned int* index)
{
    int i = 0;
    bool found = false;
    while (!found && i < count)
    {
        if (thingToFind == array[i])
        {
            found = true;
            *index = i;
        }
        else
        {
            i++;
        }
    }

    return found;

}

//gets the size of object in memory
template <typename T>
unsigned int rarray<T> :: memSize() const
{
    return sizeof(array[0]) * count;
}