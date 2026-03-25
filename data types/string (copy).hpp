string :: string()
{
    isHappeningInString = true;
    //charArray.clear();
    //charArray.clear();
    //charArray.resize(20);
    //charArray.clear();
    //charArray.resize(10);
    //charArray = dynarray<char>();
    *(char*)0xB8028 = 'A';
	*(char*)0xB8029 = 0x0C;
    //breakpointHack();
    // charArray = new char[1];
    // charArray[0] = 0;
    // dynarray<char> testTemp;
    // charArray = testTemp;
    // charArray = dynarray<char>();
    // charArray = (dynarray)malloc(10);
    // charArray();

    printf((unsigned int)charArray.getAddress(), 0xB8026, 0x0E);
    printf((unsigned int)charArray.max_size(), 0xB80C6, 0x0E);
    printf((unsigned int)charArray.getSize(), 0xB80B0, 0x0E);
    
    E9_stringCreateDebug((unsigned int)charArray.max_size(), '0', charArray.getAddress());
}

string :: string(const char *array)
{
    //isHappeningInString = true;
    //char* arrayTestCheck = array;
    //char charToAdd = array[0];
    //*(char*)0xB8024 = 'S';
	//*(char*)0xB8025 = 0x0E;
    //charArray.clear();  //I am skepical about this "fix" but putting it here did at least seem to stop bugs.. for now
    //charArray.resize(10);  //an ugly hack but it does stop all the issues
    *(char *)0xB8026 = 'S';
    //charArray.clear();
    //charArray.resize(10);
    //*(char*)0xB8027 = 0x0E;
    //globalDevVar = (unsigned int)array;
    int i = 0;
    //for (int i = 0; i < 7; i++)
    while(array[i] != 0)
    {
        asmOutb(array[i], 0xE9);
        char charToAdd = array[i];
        charArray.push_back(charToAdd);
        // charToAdd = array[i];
        i++;
    }

    //different color A depending on which constructor ran. yellow if this one
    //*(char*)0xB8028 = 'A';
	//*(char*)0xB8029 = 0x0E;
    //unsigned int oldv = (unsigned int)videoStart;
    //printf((unsigned int)charArray.getAddress());
    //printf((unsigned int)charArray.max_size(), 0xB80C6);
    //printf((unsigned int)charArray.getSize(), 0xB80B0);
    //breakpointHack();
    //videoStart = (char*)oldv;
    //isHappeningInString = true;
    //breakpointHack();
    E9_stringCreateDebug((unsigned int)charArray.max_size(), '1', charArray.getAddress());


    return;
}

string :: string(void* array)
{
    //isHappeningInString = true;
    //*(char*)0xB8024 = 'A';
	//*(char*)0xB8025 = 0x0E;
    //charArray.clear();  //I am skepical about this "fix" but putting it here did at least seem to stop bugs.. for now
    //*(char*)0xB8026 = 'A';
	//*(char*)0xB8027 = 0x0E;
    //charArray.clear();
    //charArray.resize(10);
    int i = 0;
    //for (int i = 0; i < 7; i++)
    char charToAdd = *(char*)array;
    while(charToAdd != 0)
    {
        charToAdd = *(char*)(array+i);//changed from charToAdd = *(char*)(array+i); because a cross compiler didnt like it. 10/22/23: changed back to *(char*)(array+i) because this is probably more correct
        charArray.push_back(charToAdd);
        //charToAdd = array[i];
        i++;
    }

    //different color A depending on which constructor ran
    *(char*)0xB8028 = 'A';
	*(char*)0xB8029 = 0x0D;
    //breakpointHack();
    E9_stringCreateDebug((unsigned int)charArray.max_size(), '2', charArray.getAddress());

    return;
}

void string :: manual_delete()
{
    //10/22/23 commented this out. maybe the object deletes itself when the string is deleted
    //free(&charArray);
}

char& string :: at(unsigned int index) const
{
    /*if (index < charArray.getSize())
    {
        return charArray.at(index);
    }
    else return 0;*/
    return charArray.at(index);
}

//removes last letter
void string :: pop()
{
    charArray.pop_back();
    return;
}

//fuck. there has got to be a way to get usful debug information in some way that doesn't involve a serial isa card on real hardware..
string string :: substr(unsigned int pos, unsigned int len) const
{
    //breakpointHack();
    string temp = "";
    //printMemoryAllocation();
    //breakpointHack();
    if (pos == length())
    {
        //breakpointHack();
        return "";
    }
    else if (pos > length())
    {
        //breakpointHack();
        return "out_of_range";
    }
    else
    {
        for (int i = pos; (i < (pos+len)) && i < length(); i++)
        {
            char ttt = charArray.at(i);
            temp+=ttt;
        }

        //printMemoryAllocation();
        //breakpointHack();
        return temp;
    }


    /*string temp = "";
    if (pos == len) return "";
    else if (pos > len) return "out_of_range";
    else
    {
        for (int i = pos; i < len + 1; i++)
        {
            temp+=charArray.at(i);
        }

        return temp;
    }*/
}

bool string :: contains(string s2)
{
    int i = 0;
    bool foundMatch = false;
    while (i < length() && !foundMatch)
    {
        int c = i;
        int p = 0;
        bool stillEqual = true;
        while(p < s2.length() && c < length() && stillEqual)
        {
            if (charArray.at(c) == s2[p])
            {
                stillEqual = true;
            }
            else
            {
                stillEqual = false;
            }
            c++;
            p++;
        }

        //check if a match was found
        if (stillEqual) foundMatch = true;
        i++;
    }

    return foundMatch;
}

char& string :: operator[](const unsigned int index)
{
    return charArray.at(index);
}

void string :: manual_clear()
{
    charArray.clear();
    return;
}

string& string :: operator=(const string& other)
{
    E9_strcpy((void*)charArray.getAddress());
    //*(char*)0xB8030 = 'E';
	//*(char*)0xB8031 = 0x0E;
    charArray.clear();
    charArray.resize(10);
    //*(char*)0xB8031 = 'E';
	//*(char*)0xB8032 = 0x0E;;
    for (int i = 0; i < other.length(); i++)
    {
        charArray.push_back(other.at(i));
        //*(char *)(0xB8000 + (i * 2)) = other.at(i);
        //*(char *)(0xB8001 + (i * 2)) = 0x0E;
    }

    //charArray = other.charArray;

    return *this;
}

string& string :: operator=(const char* other)
{
    //its being set equal to something and everything else is being discarded
    charArray.clear();
    charArray.resize(10);
    int i = 0;
    while (other[i] != 0)
    {
        charArray.push_back(other[i]);
        i++;
    }

    return *this;
}

string& string :: operator+(const char* other)
{
    /*int i = 0;
    while(other[i] != 0)
    {
        charArray.push_back(other[i]);
        i++;
    }*/

    return *this+=other;
}

string& string :: operator+(const string& other)
{
    /*int i = 0;
    for (int i = 0; i < other.length(); i++)
    {
        charArray.push_back(other.at(i));
    }*/

    return *this+=other;
}

string& string :: operator+(const char other)
{
    return *this+=other;
}

string& string :: operator+=(const char* other)
{
    int i = 0;
    while(other[i] != 0)
    {
        charArray.push_back(other[i]);
        i++;
    }

    return *this;
}

string& string :: operator+=(const char other)
{
    charArray.push_back(other);

    return *this;
}

string& string :: operator+=(const string other)
{
    int i = 0;
    for (int i = 0; i < other.length(); i++)
    {
        charArray.push_back(other.at(i));
    }

    return *this;
}

bool operator==(const string& lhs, const string& rhs)
{
    if (lhs.length() != rhs.length()) return false;
    else
    {
        bool stillEqual = true;
        int i = 0;
        while (stillEqual && i < lhs.length())
        {
            if (lhs.at(i) != rhs.at(i)) stillEqual = false;
            i++;
        }

        return stillEqual;
    }
}

bool operator==(const char* lhs, const string& rhs)
{
    //get length of char array by figuring out where the null terminator is
    int y = lengthOfCharArray(lhs);

    //if they're different sizes, they aren't equal
    if (y != rhs.length()) return false;
    else
    {
        bool stillEqual = true;
        int i = 0;
        while (stillEqual && i < y)
        {
            if (lhs[i] != rhs.at(i)) stillEqual = false;
            i++;
        }

        return stillEqual;
    }
}

bool operator==(const string& lhs, const char* rhs)
{
    // get length of char array by figuring out where the null terminator is
    int y = lengthOfCharArray(rhs);

    //if they're different sizes, they aren't equal
    if (y != lhs.length()) return false;
    else
    {
        bool stillEqual = true;
        int i = 0;
        while (stillEqual && i < y)
        {
            if (rhs[i] != lhs.at(i)) stillEqual = false;
            i++;
        }

        return stillEqual;
    }
}

unsigned int lengthOfCharArray(const char* array)
{
	int i = 0;
	char c = array[0];
	while (c != 0)
	{
		i++;
		c = array[i];
	}

	return i;
}