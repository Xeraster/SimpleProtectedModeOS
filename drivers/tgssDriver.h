#ifndef TGSSDRIVER_H
#define TGSSDRIVER_H

//a basic driver for the fpoga video card. the TGSS standard. (This Graphics Standard Sucks)

void drawPixel_tgss(unsigned short x, unsigned short y, unsigned short color);

void drawTestPattern_tgss();

//bresenham's line algorithm. Draw a line in 640x480x16bit mode
void bhm_line_tgss(int x1, int y1, int x2, int y2, unsigned short color);

#include "tgssdriver.hpp"

#endif