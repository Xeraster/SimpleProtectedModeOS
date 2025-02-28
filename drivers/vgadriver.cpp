/*;note: host system is supposed to store the following vga values at the following ram locations:
;   449 = current video mode
;   44A = number of columns on screen
;   44C = length of active screen buffer
;   44E = start of current page
;   450 = cursor positions
;   460 = cursor mode
;   462 = current display page
;   463 = base host address of port
;   465 = current mode
;   466 = current color
;   484 = number of character rows
*/

//relies on up to date vga mode information being in ram at 449h
void resOfVGAMode(int *x, int *y)
{
    char mode = *(char *)0x449;

    if (mode == 3)          //16 color 80x25 text mode
    {
        *x = 80;
        *y = 25;
    }
    else if (mode == 0xD)   //16 color 320x200 graphics mode
    {
        *x = 320;
        *y = 200;
    }
    else if (mode == 0x10)  //16 color 640x350 graphics mode
    {
        *x = 640;
        *y = 350;
    }
    else if (mode == 0x12)  //16 color 640x480 graphics mode
    {
        *x = 640;
        *y = 480;
    }
    else if (mode == 0x13)  //256 color 320x200 graphics mode
    {
        *x = 320;
        *y = 200;
    }

    return;
}

char getvgamode()
{
    return *(char *)0x449;
}