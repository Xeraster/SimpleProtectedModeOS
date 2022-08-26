//visual studio code is absolutely having a cow over this template business but it compiles so idgaf

template <typename T>
dynarray<T> :: dynarray()
{
    array = (T*)calloc(10, sizeof(T));
    count = 0;
    maxSize = 10;
    m_isMemoryTable = false;
    return;
}

//memory allocation not set up yet? Manually specify a location in memory to put the first dynarray
template <typename T>
dynarray<T> :: dynarray(unsigned int manual_allocation)
{
    //make the array become a pointer 
    //array = (T*)calloc(10, sizeof(T));
    array = (T*)manual_allocation;
    count = 0;
    maxSize = 10;
    m_isMemoryTable = true;
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
void dynarray<T> :: manual_resize(unsigned int newMaxSize, void* newArrayAddress)
{
    array = (T*)newArrayAddress;
    maxSize = newMaxSize;
}

template <typename T>
void dynarray<T> :: push_back(T item, bool normalOperation)
{
    //if count is less than max size, just simply apply the data to the highest position and call it a day
    //if it's the memory allocator, it needs to resize 2 spaces before it gets empty since the process of resizing itself takes 2 additional spaces
    if (count + 3 >= maxSize && m_isMemoryTable && normalOperation)
    {
        realloc(this, maxSize + 10);
        //resize(maxSize + 10);
        //array[count] = item;
        //count++;

        //remember to apply the new data or it will get desynced
        array[count] = item;
        count++;
    }
    else if (count < maxSize)
    {
        array[count] = item;
        count++;
    }
    else
    {
        resize(maxSize + 10);
        array[count] = item;
        count++;
    }

    return;
}

//delete the last element
template <typename T>
void dynarray<T> :: pop_back()
{
    if (count > 0)
    {
        count--;
    }
}

//inserts an item at designated position
template <typename T>
void dynarray<T> :: insert(unsigned int position, T item)
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
void dynarray<T> :: erase(unsigned int position)
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
void dynarray<T> :: clear()
{
    //I mean, right? Am I missing something or is this all there is to it?
    count = 0;
    resize(10);
}

//makes the data object become the same size as newSize
template <typename T>
void dynarray<T> :: resize(unsigned int newSize)
{
    maxSize = newSize;
    unsigned int newSizeInBytes = sizeof(T)*maxSize;
    void* oldPtr = array;
    array = (T*)realloc(array, newSizeInBytes);
    
    //don't forget to delete the old array
    free(oldPtr);

    return;
}

//changes the size of the data object to exactly contain everything and no more
template <typename T>
void dynarray<T> :: shrink_to_fit()
{
    //it needs a buffer of like 5 or it won't work for some reason
    resize(count+5);
}

//returns whatever element is at front
template <typename T>
T& dynarray<T> :: front()
{
    return array[0];
}

//returns whatever element is at back
template <typename T>
T& dynarray<T> :: back()
{
    if (count > 0) return array[count-1];
    else return *(T*)nullptr;
}

template <typename T>
bool dynarray<T> :: find(T thingToFind, unsigned int* index)
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
unsigned int dynarray<T> :: memSize() const
{
    return sizeof(array[0]) * count;
}