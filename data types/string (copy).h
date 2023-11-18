#ifndef STRING_H
#define STRING_H

class string
{
public:
    string();
    string(const char *array);
    string(void* array);
    ~string()
    {
        //E9_stringDeleteDebug(charArray.max_size(), 'd', charArray.getAddress());
        //charArray.manual_delete();
        asmOutb('O', 0xE9);
    }

    //~string causes errors but those errors are easily circumvented with this
    void manual_delete();

    unsigned int length() const { return charArray.getSize(); }
    char& at(unsigned int index) const;
    char at_noconst(unsigned int index) { return charArray.at_noconst(index); }

    //removes last letter
    void pop();

    //the substring function. look it up.
    string substr(unsigned int pos, unsigned int len) const;

    //returns true if the string contains the given string. Returns false if it does not
    bool contains(string s2);

    //enough of that no [] operator bullshit.
    char& operator[](const unsigned int index);

    //its a long shot, but maybe using this instead of setting equal to " " will make it not completely fuck out
    void manual_clear();

    string& operator=(const string& other);
    string& operator=(const char* other);

    //this works the same way the = operator does except it doesn't clear anything beforehand
    string& operator+(const char* other);
    string& operator+(const char other);
    string& operator+(const string& other);

    string& operator+=(const char* other);
    string& operator+=(const char other);
    string& operator+=(const string other);

    unsigned int arrayAddress() const { return (unsigned int)charArray.getAddress(); }
    unsigned int containerSize() const { return charArray.max_size(); }

private:
    dynarray<char> charArray;
};

//non-member class specific operators
bool operator==(const string& lhs, const string& rhs);
bool operator==(const char* lhs, const string& rhs);
bool operator==(const string& lhs, const char* rhs);

//returns the length of a null terminated character array
unsigned int lengthOfCharArray(const char* array);

#include "string.hpp"

#endif
