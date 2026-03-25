//equivalents of all the std stuff, implemented as needed
#ifndef STANDARD_H
#define STANDARD_H

//good for base 10 numbers up to ~2.1 million. Negative numbers not supported right now. idx variable also not supported
int stoi(const string& str, unsigned int* idx = 0, int base = 10)
{
    int amtToMult = 1;     //by how much to multiply the loop by each time
    int numSoFar = 0;
    char foundChars[10];

    if (base == 10)
    {
        for (int i = str.length(); i > 0; i--)
        {
            char thing = str.at(i - 1);
            thing = thing - 48;
            numSoFar += thing * amtToMult;
            amtToMult = amtToMult * 10;
        }
    }
    else        //should be good enough for non base 10 stuff most of the time
    {
        for (int i = str.length(); i > 0; i--)
        {
            char thing = str.at(i - 1);
            
            //thing = thing - 48;
            if (thing >= 97)
            {
                thing -= 32;
            }
            if (thing >= 65)
            {
                thing -= 55;
            }
            else
            {
                thing -= 48;
            }
            
            numSoFar += thing * amtToMult;
            amtToMult = amtToMult * base;
        }
    }

    return numSoFar;
}

#endif