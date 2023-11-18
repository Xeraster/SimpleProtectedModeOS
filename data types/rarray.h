#ifndef RARRAY_H
#define RARRAY_H

//rarray is just a dynarray without the ability to memory manage itself
template <typename T>
class rarray
{
public:
    rarray();

    rarray(const rarray<T>& other)
    {
        asmOutb('k',0xE9);
        void* oldPtr = array;
        count = 0;
        maxSize = 10;
        free(array);
        array = (T*)malloc(maxSize * sizeof(T));
        m_init = true;
        for (int i = 0; i < other.getSize(); i++)
        {
            push_back(other.at(i));
        }
    }


    //deletion operator
    ~rarray()
    {
        count = 0;
        maxSize = 0;
        free(array);
    }

    //getting delete and new operators to work is fucking impossible so use this workaround. want to refute that point? Then prove otherwise. Remember: no outside libraries unless you can make them actually compile and run on target hardware (486sx)
    void manual_delete()
    {
        //count = 0;
        //maxSize = 0;
        free(array);
        return;
    }

    //used by realloc when attempting to realloc the memory allocation table dynarrays
    void manual_resize(unsigned int newMaxSize, void* newArrayAddress);
    
    //try to make the commonly used functions have the same names and take the same parameters as the og vector ones do
    //maybe omit the wierd iterator business though. This data type is for stability. Speed is a secondary concern

    //attaches an item to the back of the array
    void push_back(T item);

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

    rarray<T>& operator=(const rarray<T>& other)
    {
        asmOutb('p',0xE9);
        void* oldPtr = array;
        count = 0;
        maxSize = 10;
        free(array);
        array = (T*)malloc(maxSize * sizeof(T));
        //array = other.getAddress();
        //maxSize = other.max_size();
        //m_isMemoryTable = other.is_memory_table();
        m_init = other.is_init();
        //slow_memcpy(array, oldPtr, maxSize * sizeof(T));
        for (int i = 0; i < other.getSize(); i++)
        {
            push_back(other.at(i));
        }

        
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

            //fuck
            //slow_memcpy(array, other.array, other.count * sizeof(T));
        }*/

        return *this;
    }

    //i don't think i'll end up finding a way to get this working
    void* operator new(unsigned int size)
    {

        /**(char*)0xB8012 = 'N';
	    *(char*)0xB8013 = 0x0E;
        array = (T*)calloc(10, sizeof(T));
        *(char*)0xB8014 = 'U';
	    *(char*)0xB8015 = 0x0E;
        count = 0;
        maxSize = 10;
        m_isMemoryTable = false;
        m_init = true;*/

        //return (T*)calloc(10, sizeof(T));
        //void* ptr = (void)rarray<T>();
        
        //really ugly hack but there seems to be no other way
        /*rarray<T> newArray = rarray<T>();
        void *ptr = (T*)malloc(newArray.memSize());
        memcpy(ptr, (void*)newArray, newArray.memSize()); //now thje rarray I created can be deleted*/

        //there just isn't a way to do this is there
    
        intToE9(size, false);
        asmOutb('p', 0xE9);
        asmOutb('P', 0xE9);
        //return ptr;

        //return (void*)rarray<T>();
        //return malloc
        //__asm__("hlt");
        //wizard-7b ai said to do this
        return malloc(size);
    }

    //void* newRarray()

    /*void* operator delete(void* ptr)
    {

    }*/

    unsigned int count;
private:
    unsigned int maxSize;
    bool m_isMemoryTable;
    bool m_init;
    T *array;

    //gets the size of object in memory
    unsigned int memSize() const;
};

#include "rarray.hpp"

#endif