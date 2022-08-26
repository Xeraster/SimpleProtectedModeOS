string :: string()
{
    //charArray = new char[1];
    //charArray[0] = 0;
    //charArray = new dynarray<char>();
    //charArray = dynarray<char>();
}

string :: string(char array[])
{
    //char* cum = array;
    //char charToAdd = array[0];
    globalDevVar = (unsigned int)array;
    for (int i = 0; i < 7; i++)
    {
        charArray.push_back(array[i]);
        //charToAdd = array[i];
    }
}

string :: string(void* array)
{
    //char* cum = array;
    char charToAdd = *(char*)array;
    for (int i = 0; i < 7; i++)
    {
        charArray.push_back(charToAdd);
        charToAdd = *(char*)(array+i);
    }
}

char string :: at(unsigned int index) const
{
    /*if (index < charArray.getSize())
    {
        return charArray.at(index);
    }
    else return 0;*/
    return charArray.at(index);
}

string& string :: operator=(const string& other)
{

}

string& string :: operator=(const char* other)
{
    //charArray.clear();
    for (int i = 0; i < 7; i++)
    {
        charArray.push_back('F');
    }

    return *this;
}