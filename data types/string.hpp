string :: string()
{
    //charArray.clear();
    //charArray.clear();
    //charArray.resize(20);
    //charArray.clear();
    //charArray.resize(10);
    //charArray = dynarray<char>();
    //*(char*)0xB8028 = 'A';//
	//*(char*)0xB8029 = 0x0C;//
    //breakpointHack();
    // charArray = new char[1];
    // charArray[0] = 0;
    // dynarray<char> testTemp;
    // charArray = testTemp;
    // charArray = dynarray<char>();
    // charArray = (dynarray)malloc(10);
    // charArray();

    //printf((unsigned int)charArray.getAddress(), 0xB8026, 0x0E);
    //printf((unsigned int)charArray.max_size(), 0xB80C6, 0x0E);
    //printf((unsigned int)charArray.getSize(), 0xB80B0, 0x0E);
    //E9_stringCreateDebug(charArray.max_size(), '0', (void*)charArray.getAddress());
}

string :: string(const string &str)
{
    //asmOutb('L', 0xE9);
    //E9_strcpy((void*)charArray.getAddress());
    //*(char*)0xB8030 = 'E';
	//*(char*)0xB8031 = 0x0E;
    charArray.clear();
    //charArray.resize(10);
    //*(char*)0xB8031 = 'E';
	//*(char*)0xB8032 = 0x0E;;
    for (int i = 0; i < str.length(); i++)
    {
        charArray.push_back(str.at(i));
        //*(char *)(0xB8000 + (i * 2)) = other.at(i);
        //*(char *)(0xB8001 + (i * 2)) = 0x0E;
    }
    //E9_stringCreateDebug(charArray.max_size(), '1', (void*)charArray.getAddress());
}

string :: string(const char *array)
{
    *(char*)0xB8048 = 'S';
	*(char*)0xB8049 = 0x0A;
    //isHappeningInString = true;
    //char* arrayTestCheck = array;
    //char charToAdd = array[0];
    //*(char*)0xB8024 = 'S';
	//*(char*)0xB8025 = 0x0E;
    //charArray.clear();  //I am skepical about this "fix" but putting it here did at least seem to stop bugs.. for now
    //charArray.resize(10);  //an ugly hack but it does stop all the issues
    //*(char *)0xB8026 = 'S';
    charArray.clear();
    //charArray.resize(10);
    //*(char*)0xB8027 = 0x0E;
    //globalDevVar = (unsigned int)array;
    int i = 0;
    //for (int i = 0; i < 7; i++)
    while(array[i] != 0)
    {
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
    //E9_stringCreateDebug(charArray.max_size(), '2', (void*)charArray.getAddress());

    *(char*)0xB8048 = 0x00;
	*(char*)0xB8049 = 0x00;



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

    //E9_stringCreateDebug(charArray.max_size(), '3', (void*)charArray.getAddress());


    return;
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

//it's possible to use port E9 to get debug information in bochs
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

char * string :: c_str() const
{
    return charArray.getAddress();
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
    charArray.clear();
    *(char*)0xB8030 = 'E';
	*(char*)0xB8031 = 0x0E;
    //charArray.clear();
    //charArray.resize(10);
    *(char*)0xB8031 = 'E';
	*(char*)0xB8032 = 0x0E;;
    for (int i = 0; i < other.length(); i++)
    {
        charArray.push_back(other.at(i));
        *(char *)(0xB8000 + (i * 2)) = other.at(i);
        *(char *)(0xB8001 + (i * 2)) = 0x0E;
        //commenting this these debug strings doesn't improve speed in any meaningful way
    }

    //charArray = other.charArray;

    return *this;
}

string& string :: operator=(const char* other)
{
    //its being set equal to something and everything else is being discarded
    charArray.clear();
    //charArray.resize(10);
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

string& string :: operator+=(const string &other)
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
    asmOutb('\n', 0xE9);
    asmOutb('a', 0xE9);
    asmOutb('a', 0xE9);
    asmOutb('\n', 0xE9);
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

        for (int o = 0; o < lhs.length(); o++)
        {
            asmOutb('[', 0xE9);
            asmOutb(rhs[o], 0xE9);
            asmOutb('.', 0xE9);
            asmOutb(lhs.at(o), 0xE9);
            asmOutb(']', 0xE9);
            asmOutb(' ', 0xE9);
        }
        asmOutb('\n', 0xE9);
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

//basically the equivalent to python str split function
//uses char arrays so you will need to free the pointer when done using it
//rarray<string> split(string input, char delimiter)
void split(string input, char delimiter, rarray<string> *splitsSoFar)
{
    //never going to happen. You can't have the new operator in a non mac/linux/windows system even if you have a working memory manager.
    //rarray<string> splitsSoFar = rarray<string>();
    //asmOutb('\n', 0xE9);
    //splitsSoFar->clear();   //the closest to working I could get the "new" operator was a really dumb hack that chatgpt shat out
    //intToE9((unsigned int)splitsSoFar, false);
    //asmOutb('\n', 0xE9);
    //__asm__("hlt");
    /*if (!allocationExists(splitsSoFar))
    {
        asmOutb('e', 0xE9);
        asmOutb('r', 0xE9);
        asmOutb('r', 0xE9);
        asmOutb('o', 0xE9);
        asmOutb('r', 0xE9);
    }
    asmOutb('y', 0xE9);
    asmOutb('a', 0xE9);
    asmOutb('y', 0xE9);*/
    splitsSoFar->clear();
    string soFar = "";
    for(int i = 0; i < input.length(); i++)
    {
        if (input.at(i) != delimiter)
        {
            soFar += input.at(i);
        }
        else
        {
            //pushing back this string seems to be the cause of The C++ Bug
            splitsSoFar->push_back(soFar);
            soFar = "";
            //soFar.manual_clear();
        }
    }

    //now copy the string to a char array pointer.. oh wait that won't work. I mean it kinda would but preventing memory leaks while doing it this way would be really hard probably
    //meh, just return the resize array object
    //return splitsSoFar;
}