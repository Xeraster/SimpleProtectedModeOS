#ifndef MATH_H
#define MATH_H

#include "math.hpp"

int pow_slow(int input, int pow);

//takes an integers less than 16 and converts it to the ascii value for whatever that int is in hex
char intToHexChar(int input);

unsigned int betterRand(unsigned int seed);

//return the absolute value of the given number. Intended to be the same as the std math abs
unsigned int abs(int num);

#endif