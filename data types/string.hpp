string :: string()
{
    //charArray.clear();
    //charArray = new char[1];
    //charArray[0] = 0;
    //dynarray<char> testTemp;
    //charArray = testTemp;
    //charArray = dynarray<char>();
    //charArray = (dynarray)malloc(10);
    //charArray();
}

string :: string(const char *array)
{
    //char* arrayTestCheck = array;
    //char charToAdd = array[0];
    *(char*)0xB8024 = 'S';
	*(char*)0xB8025 = 0x0E;
    charArray.clear();  //I am skepical about this "fix" but putting it here did at least seem to stop bugs.. for now
    *(char*)0xB8026 = 'S';
	*(char*)0xB8027 = 0x0E;
    //globalDevVar = (unsigned int)array;
    int i = 0;
    //for (int i = 0; i < 7; i++)
    while(array[i] != 0)
    {
        charArray.push_back(array[i]);
        //charToAdd = array[i];
        i++;
    }

    *(char*)0xB8300 = 0xFF;
    *(char*)0xB8301 = 0xFF;
    *(char*)0xB8302 = 0xFF;
    *(char*)0xB8303 = 0xFF;

    return;
}

string :: string(void* array)
{
    *(char*)0xB8024 = 'A';
	*(char*)0xB8025 = 0x0E;
    charArray.clear();  //I am skepical about this "fix" but putting it here did at least seem to stop bugs.. for now
    *(char*)0xB8026 = 'A';
	*(char*)0xB8027 = 0x0E;
    int i = 0;
    //for (int i = 0; i < 7; i++)
    char charToAdd = *(char*)array;
    while(charToAdd != 0)
    {
        charToAdd = *(char*)(array)+i;//changed from charToAdd = *(char*)(array+i); because a cross compiler didnt like it
        charArray.push_back(charToAdd);
        //charToAdd = array[i];
        i++;
    }

    *(char*)0xB8028 = 'A';
	*(char*)0xB8029 = 0x0E;

    return;
}

void string :: manual_delete()
{
    free(&charArray);
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

string string :: substr(unsigned int pos, unsigned int len) const
{
    string temp = "";
    if (pos == length())
    {
        return "";
    }
    else if (pos > length())
    {
        return "out_of_range";
    }
    else
    {
        for (int i = pos; (i < (pos+len)) && i < length(); i++)
        {
            char ttt = charArray.at(i);
            temp+=ttt;
        }

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
    *(char*)0xB8030 = 'E';
	*(char*)0xB8031 = 0x0E;
    charArray.clear();
    *(char*)0xB8031 = 'E';
	*(char*)0xB8032 = 0x0E;;
    //charArray.manual_delete();
    //charArray = dynarray<char>();
    //charArray.shrink_to_fit();
    //charArray = dynarray<char>();
    //int i = 0;
    for (int i = 0; i < other.length(); i++)
    {
        charArray.push_back(other.at(i));
    }
    *(char*)0xB8033 = 'E';
    *(char*)0xB8034 = 0x0E;

    return *this;
}

string& string :: operator=(const char* other)
{
    *(char*)0xB8030 = 'C';
	*(char*)0xB8031 = 0x0E;
    //charArray.manual_delete();
    //charArray = dynarray<char>();
    //charArray = dynarray<char>();
    *(char*)0xB8032 = 'C';
	*(char*)0xB8033 = 0x0E;
    if (other[0] == 0)
    {
        /*for (int i = 0; i < charArray.getSize(); i++)
        {
            charArray.at(i) = char(0);
        }*/
        *(char*)0xB8034 = 'c';
	    *(char*)0xB8035 = 0x0E;
        charArray.clear();
        *(char*)0xB8036 = 'c';
	    *(char*)0xB8037 = 0x0E;
    }
    else
    {
        *(char*)0xB8034 = 'C';
	    *(char*)0xB8035 = 0x0E;
        charArray.clear();
        *(char*)0xB8036 = 'C';
	    *(char*)0xB8037 = 0x0E;
        int i = 0;
        while(other[i] != 0)
        {
            charArray.push_back(other[i]);
            i++;
        }
        *(char*)0xB8038 = 'C';
	    *(char*)0xB8039 = 0x0E;
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
    //get length of char array by figuring out where the null terminator is
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