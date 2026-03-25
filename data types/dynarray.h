#ifndef DYNARRAY_H
#define DYNARRAY_H
//do not touch with any of this without testing it EXTREMELY well...

//basically a vector, just a worse one that works with my memory allocation system
//probably lighter on memory than the og vector class
//you need dynamic memory allocation in your kernel before you can make dynamic data containers. But you need at least some kind of dynamic data container before you can get dynamic memory allocation. REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
template <typename T>
class dynarray
{
public:
    dynarray();

    //memory allocation not set up yet? Manually specify a location in memory to put the first dynarray
    //it's recommended to put this in 0x0007F000 for it's initial value if trying to set up the kernel. That should give you enough space to store 1000 32-bit pointers before something bad happens.
    dynarray(unsigned int manual_allocation);

    //deletion operator
    ~dynarray()
    {
        count = 0;
        maxSize = 0;
        free(array);
    }

    //for when g++/ld is being stupid (this stopped being relevant once I figured out how to get the linker to work the way I needed it to)
    void manual_delete()
    {
        count = 0;
        maxSize = 0;
        free(array);
        return;
    }

    //used by realloc when attempting to realloc the memory allocation table dynarrays
    void manual_resize(unsigned int newMaxSize, void* newArrayAddress);
    
    //try to make the commonly used functions have the same names and take the same parameters as the og vector ones do
    //maybe omit the wierd iterator business though. This data type is for stability. Speed is a secondary concern

    //attaches an item to the back of the array
    void push_back(T item, bool normalOperation = true);

    //delete the last element
    void pop_back();

    //inserts an item at designated position
    void insert(unsigned int position, T item);

    //deletes element at position and then shifts everything down
    void erase(unsigned int position);

    //deletes everything in the data object
    void clear();

    //makes the data object become the same size as newSize
    void resize(unsigned int newSize);

    //changes the size of the data object to exactly contain everything and no more
    void shrink_to_fit();

    //returns whatever element is at front
    T& front();

    //returns whatever element is at back
    T& back();

    //returns if the thing exists or not. returns false if not found. returns true and sets value of index if found
    bool find(T thingToFind, unsigned int* index);

    unsigned int getSize() const { return count; }
    unsigned int getSize_noconst() { return count; }    //in certain situations, returning a const value here crashes the program when running on physical systems but not on emulators
    unsigned int max_size() const { return maxSize; }
    T* getAddress() const { return array; }
    bool is_init() const { return m_init; }

    bool is_memory_table() const { return m_isMemoryTable; }

    //returns point to element at position
    T& at(unsigned int index) const { return array[index]; }
    T& at_noconst(int index) { return array[index]; }

    //wow, the entire system relies on this being set up in a broken way. update 02/28/2025: that comment was referencing something from several releases ago and is no longer relevant
    dynarray<T>& operator=(const dynarray<T>& other)
    {
        //the system won't even run without this block of code but this block of code is stupid and wrong
            //02/28/2025: looks good to me ¯\_(ツ)_/¯
            free(array);
            count = other.getSize();
            array = other.getAddress();
            maxSize = other.max_size();
            m_isMemoryTable = other.is_memory_table();
            m_init = other.is_init();
        
        /*if (other.is_memory_table())
        {
            free(array);
            count = other.getSize();
            array = other.getAddress();
            maxSize = other.max_size();
            m_isMemoryTable = other.is_memory_table();
            m_init = other.is_init();
        }
        else
        {
            free(array);
            array = (T *)calloc(other.getSize(), sizeof(T));
            count = other.getSize();
            maxSize = other.max_size();
            m_isMemoryTable = other.is_memory_table();
            m_init = other.is_init();

            for (int i = 0; i < other.getSize(); i++)
            {
                push_back(other.at(i));
            }

            //slow_memcpy(array, other.array, other.count * sizeof(T));
        }*/

        return *this;
    }
    unsigned int count;
private:
    unsigned int maxSize;
    bool m_isMemoryTable;
    bool m_init;
    T *array;

    //gets the size of object in memory
    unsigned int memSize() const;
};

#include "dynarray.hpp"

#endif