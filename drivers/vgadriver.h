#ifndef VGADRIVER_H
#define VGADRIVER_H

char black = 0x00;
char blue = 0x01;

//relies on up to date vga mode information being in ram at 449h
void resOfVGAMode(int *x, int *y);

char getvgamode();

extern "C" void vga_editPaletteReg(char registerNumber, char newValue);
extern "C" char vga_getModeControl();

#include "vgadriver.cpp"

#endif