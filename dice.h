/********************************************************
Author: Christopher Karpyszyn
Created: Oct 7th 2007
Version 0.5

This is a simple dice class.
It uses a call to the machine time (time(NULL) as a seed.

MUST ALWAYS CALL
srand(time(NULL));
WHEN INSTANTIATING THE DICE CLASS TO SET THE SEED NUMBER
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
	private:
		int roll1, roll2, roll3, roll4;
		int min;
		void setCharacterRolls (int, int, int, int);
	 public:
		int characterRoll();
		int roll(int);		
	};

	//This is a simple accessor to change the variables quickly in character roll.
	extern void setCharacterRolls (int, int, int, int);

	//input: the number of faces on the dice
	//output: a random roll
	extern int roll(int);

	//function makes four rolls, drops the smallest one and adds the highest three
	//output is the sum of max three rolls
	extern int characterRoll();

#endif