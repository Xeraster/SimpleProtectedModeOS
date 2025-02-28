void drawPixel_tgss(unsigned short x, unsigned short y, unsigned short color)
{
    //convert the provided x,y coordinates to linear vram coordinates
    unsigned int addressToWrite = (y * 640) + x;
    unsigned char lowByte = addressToWrite & 0xFF;
    unsigned char middleByte = (addressToWrite >> 8) & 0xFF;
    unsigned char highByte = (addressToWrite >> 16) & 0xFF;

    //write each address byte. do it twice just for good measure
    outb(lowByte, 0x428);
    outb(lowByte, 0x428);
    outb(middleByte, 0x429);
    outb(middleByte, 0x429);
    outb(highByte, 0x42A);
    outb(highByte, 0x42A);

    //write the 2 byte pixel and pray to God that it works
    outw(color, 0x42C);
}

//draw a test pattern to the fpga video card in 640x480 mode
void drawTestPattern_tgss()
{
    unsigned short colorCount = 0;
    for (int i = 10; i< 266; i++)
    {
        bhm_line_tgss(40+i, 10, 40+i, 100, colorCount);
        colorCount+=255;    //65535 / 255 = 255, so good enough
    }
}

//bresenham's line algorithm. Draw a line in 300x200 mode. Will probably eventually get modified to not be vga-specific since it's just going to call drawPixel functions
void bhm_line_tgss(int x1, int y1, int x2, int y2, unsigned short color)
{
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1;
    dy = y2 - y1;
    dx1 = abs(dx);
    dy1 = abs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            x = x1;
            y = y1;
            xe = x2;
        }
        else
        {
            x = x2;
            y = y2;
            xe = x1;
        }
        drawPixel_tgss(x, y, color);
        //DrawToSurface(x, y, theColor, surf);
        for (i = 0; x < xe; i++)
        {
            x = x + 1;
            if (px < 0)
            {
                px = px + 2 * dy1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    y = y + 1;
                }
                else
                {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            drawPixel_tgss(x, y, color);
            //DrawToSurface(x, y, theColor, surf);
        }
    }
    else
    {
        if (dy >= 0)
        {
            x = x1;
            y = y1;
            ye = y2;
        }
        else
        {
            x = x2;
            y = y2;
            ye = y1;
        }
        drawPixel_tgss(x, y, color);
        //DrawToSurface(x, y, theColor, surf);
        for (i = 0; y < ye; i++)
        {
            y = y + 1;
            if (py <= 0)
            {
                py = py + 2 * dx1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    x = x + 1;
                }
                else
                {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            //DrawToSurface(x, y, theColor, surf);
            drawPixel_tgss(x, y, color);
        }
    }
}