#ifndef STRING_H
#define STRING_H

class string
{
public:
    string();
    string(char array[]);
    string(void* array);

    unsigned int length() const { return charArray.getSize(); }
    char at(unsigned int index) const;

    string& operator=(const string& other);
    string& operator=(const char* other);
private:
    dynarray<char> charArray;
};

#include "string.hpp"

#endif
