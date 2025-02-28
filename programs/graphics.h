#ifndef GRAPHICS_H
#define GRAPHICS_H

//converts character font information into graphics mode compatible data in 256 color mode where it's 1 byte per pixel
void* charInformationToSprite256(short letter, short color, short backgroundColor, unsigned char *fontTable)
{
    void *charData = malloc(64);//an uncompressed 8 bit color 8x8 font sprite is 64 bytes in size
    unsigned int offset = (letter - 32)*8;//the fonts start at spacebar which is the 32nd ascii character. oh boy a mul opcode that'll be slow.
    //unsigned int offset = 8;

    //process each bit and figure out which pixels to set to what
    unsigned int ctr = 0;       //it's faster to do this than to multiply y*8 each time probably
    unsigned int charPos = 0;
    for (int y = 0; y < 8; y++)
    {
        //char pixels[8];
        for (int x = 0; x < 8; x++)
        {
            unsigned char theNum = (*(char*)(fontTable+offset+ctr));
            theNum = theNum & (0x01<<x);
            //if the bit at the queried position is a 0, draw the background color there. otherwise, draw the font color
            if (theNum > 0)
            {
                //*(char*)(charData+offset+ctr) = backgroundColor;
                *(char*)(charData+charPos) = color;
            }
            else
            {
                //*(char*)(charData+offset+ctr) = color;
                *(char*)(charData+charPos) = backgroundColor;
            }
            charPos++;
        }
        ctr++;
    }

    return charData;
}

//convert font data to the correct format for the current bit depth and video mode and then draw it to the screen
void graphicsModeDrawText(unsigned int posX, unsigned int posY, short letter, short color, short backgroundColor, bool transparentBackground = false)
{
    void *fontSprite = charInformationToSprite256(letter, color, backgroundColor, vgafont8);
    //void *fontSprite = (void*)(vgafont8+8);
    //unsigned int x = 20;
    //unsigned int y = 20;
    unsigned int ctr = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            //this might work
            drawPixel256(x+posX, y+posY, *(char*)(fontSprite+ctr));
            ctr++;
        }
    }

    //avoid memory leaks
    free(fontSprite);
}

//converts character font information into graphics mode compatible data in 65535 color mode where it's 2 bytes per pixel. good fucking luck with this one
void* charInformationToSprite65535(short letter, unsigned short color, unsigned short backgroundColor, unsigned char *fontTable)
{
    void *charData = malloc(128);//an uncompressed 16 bit color 8x8 font sprite is 128 bytes in size
    unsigned int offset = (letter - 32)*8;//the fonts start at spacebar which is the 32nd ascii character. the fonts are stored as 1 bit per pixel, so each font character is 8 bytes in size
    //unsigned int offset = 8;

    //process each bit and figure out which pixels to set to what
    unsigned int ctr = 0;       //it's faster to do this than to multiply y*8 each time probably
    unsigned int charPos = 0;
    for (int y = 0; y < 8; y++)
    {
        //char pixels[8];
        for (int x = 0; x < 8; x++)
        {
            unsigned char theNum = (*(char*)(fontTable+offset+ctr));
            theNum = theNum & (0x01<<x);
            //if the bit at the queried position is a 0, draw the background color there. otherwise, draw the font color
            if (theNum > 0)
            {
                //*(char*)(charData+offset+ctr) = backgroundColor;
                *(unsigned short*)(charData+(charPos*2)) = color;
            }
            else
            {
                //*(char*)(charData+offset+ctr) = color;
                *(unsigned short*)(charData+(charPos*2)) = backgroundColor;
            }
            charPos++;
        }
        ctr++;
    }

    return charData;
}

//convert font data to the correct format for the current bit depth and video mode and then draw it to the screen. This is the one that should be used for TGSS
void graphicsModeDrawText_tgss(unsigned int posX, unsigned int posY, short letter, unsigned short color, unsigned short backgroundColor, bool transparentBackground = false)
{
    void *fontSprite = charInformationToSprite65535(letter, color, backgroundColor, vgafont8);
    //void *fontSprite = (void*)(vgafont8+8);
    //unsigned int x = 20;
    //unsigned int y = 20;
    unsigned int ctr = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            //this might work
            drawPixel_tgss(x+posX, y+posY, *(unsigned short*)(fontSprite+(ctr*2)));
            ctr++;
        }
    }

    //avoid memory leaks
    free(fontSprite);
}

#endif