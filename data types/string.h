#ifndef STRING_H
#define STRING_H

class string
{
public:
    string();
    string(const string &str);
    string(const char *array);
    string(void* array);
    ~string()
    {
        //charArray.manual_delete();
        //E9_stringDeleteDebug(charArray.max_size(), '1', (void*)charArray.getAddress());
        charArray.manual_delete();
    }

    unsigned int length() const { return charArray.getSize(); }
    char& at(unsigned int index) const;
    char at_noconst(unsigned int index) { return charArray.at_noconst(index); }

    //removes last letter
    void pop();

    //the substring function. look it up.
    string substr(unsigned int pos, unsigned int len) const;

    char * c_str() const;

    //returns true if the string contains the given string. Returns false if it does not
    bool contains(string s2);

    //enough of that no [] operator bullshit.
    char& operator[](const unsigned int index);

    //its a long shot, but maybe using this instead of setting equal to " " will make it not completely mess up in buggy situations
    void manual_clear();


    string& operator=(const string& other);
    string& operator=(const char* other);

    //this works the same way the = operator does except it doesn't clear anything beforehand
    string& operator+(const char* other);
    string& operator+(const char other);
    string& operator+(const string& other);

    string& operator+=(const char* other);
    string& operator+=(const char other);
    string& operator+=(const string& other);

    unsigned int arrayAddress() const { return (unsigned int)charArray.getAddress(); }
    unsigned int containerSize() const { return charArray.max_size(); }

private:
    rarray<char> charArray;
};

//non-member class specific operators
bool operator==(const string& lhs, const string& rhs);
bool operator==(const char* lhs, const string& rhs);
bool operator==(const string& lhs, const char* rhs);

//returns the length of a null terminated character array
unsigned int lengthOfCharArray(const char* array);

//basically the equivalent to python str split function
//uses char arrays so you will need to free the pointer when done using it
//never going to happen. You can't have the new operator in a non mac/linux/windows system even if you have a working memory manager + there's no way to prevent it from deallocating the pointer if you try to resrot to any of the other possible workarounds
//rarray<string>* split(string input, char delimiter);
void split(string input, char delimiter, rarray<string> *splitsSoFar);

#include "string.hpp"

#endif
