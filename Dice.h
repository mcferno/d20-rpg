/********************************************************
Author: Christopher Karpyszyn
Created: Oct 7th 2007
Version 0.5

This is a simple dice class.
It uses a call to the machine time (time(NULL) as a seed.

MUST ALWAYS CALL
  srand ((unsigned int)time(NULL));
Before using the dice class to seed the rand
*********************************************************/

#ifndef DICE_H
	#define DICE_H

#include <iostream>
#include <math.h>
#include <time.h>

//class dice requires no variables
//remember to call srand(time(NULL));
class Dice
{
 public:
	// all the different kinds of dice
	static const int D3 = 3;
	static const int D4 = 4;
	static const int D6 = 6;
	static const int D8 = 8;
	static const int D10 = 10;
	static const int D12 = 12;
	static const int D16 = 16;
	static const int D20 = 20;

	// rolls a given given dice a certain number of times (default: once)
	static int roll(int, int = 1);
};

#endif