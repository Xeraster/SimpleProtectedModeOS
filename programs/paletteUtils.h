#ifndef PALETTE_UTILS
#define PALETTE_UTILS

//get a vector of every line in the file and return it.
//this function is quite possibly the slowest thing in the universe
//this is really slow so it makes a good system benchmark
void getLinesOfPaletteFile(void *fileData, unsigned int size, rarray<string> *fileLines)
{
    //rarray<string> *fileLines = new rarray<string>();
    //save cpu time by resizing to almost the maximum size
    //fileLines->resize(250);      //resizing to a number larger than 10 when initializing an rarray pointer with the new keyword improves performance by a moderate amount
    //string lineSoFar = "";
    
    
    unsigned int counter = 0;
    char data = 0x00;
    while (counter < size)
    {
        //fileLines->push_back(lineSoFar);
        string lineSoFar;
        while(data != 0x0D && data != 0x0A && data != 0x09 && counter < size)//I'm pretty sure 0x0A is the one that gets used for newline most often
        {
            data = *(char*)(fileData+counter);
            //if (data > 31)
            //{
                lineSoFar+=data;
            //}
            counter++;
        }
        if (lineSoFar.length() > 10)
        {
            fileLines->push_back(lineSoFar);//not the part that takes forever
        }
        //lineSoFar = "";
        //lineSoFar.~string();
        data = 0x00;
    }
    /*for (int i = 0; i < 300; i++)
    {
        fileLines->push_back("poop");
        fileLines->push_back("pee");
    }*/

    //return fileLines;
}

//the file goes in and then a vector of 16 bit color index values comes out
//the idea is that you apply these 16 bit color values to the index color values of your 256 color index
void getValuesOfPaletteFile(void *fileData, unsigned int size, rarray<short> *colorValues)
{
    unsigned int counter = 0;
    char data = 0x00;
    while (counter < size)
    {
        //fileLines->push_back(lineSoFar);
        string lineSoFar;
        while(data != 0x0D && data != 0x0A && counter < size)//I'm pretty sure 0x0A is the one that gets used for newline most often
        {
            data = *(char*)(fileData+counter);
            if (data > 31)
            {
                lineSoFar+=data;
            }
            counter++;
        }
        if (lineSoFar.length() > 10)
        {
            //fileLines->push_back(lineSoFar);//not the part that takes forever
            //rarray<string> splitNums = rarray<string>();//seperate all the numbers
            //split(lineSoFar,)
            string Rnum;
            string Gnum;
            string Bnum;

            //isolate all the numerical values
            for (int i = 0; i < 3; i++)
            {
                if (lineSoFar[i] > 32)
                {
                    Rnum+=lineSoFar[i];
                }
            }

            for (int i = 0; i < 3; i++)
            {
                if (lineSoFar[i+4] > 32)
                {
                    Gnum+=lineSoFar[i+4];
                }
            }

            for (int i = 0; i < 3; i++)
            {
                if (lineSoFar[i+8] > 32)
                {
                    Bnum+=lineSoFar[i+8];
                }
            }
            /*Rnum += lineSoFar[0];
            Rnum += lineSoFar[1];
            Rnum += lineSoFar[2];

            Gnum += lineSoFar[4];
            Gnum += lineSoFar[5];
            Gnum += lineSoFar[6];

            Bnum += lineSoFar[8];
            Bnum += lineSoFar[9];
            Bnum += lineSoFar[10];*/

            //convert them all to numbers
            unsigned int R = 63 * stoi(Rnum) / 255;            //2^6 * x / 255;
            unsigned int G = 63 * stoi(Gnum) / 255;
            unsigned int B = 63 * stoi(Bnum) / 255;

            /*int R = stoi(Rnum);            //2^6 * x / 255;
            int G = stoi(Gnum);
            int B = stoi(Bnum);*/

            colorValues->push_back(0x0000003F&R);
            colorValues->push_back(0x0000003F&G);
            colorValues->push_back(0x0000003F&B);
        }
        //lineSoFar = "";
        //lineSoFar.~string();
        data = 0x00;
    }

}

#endif