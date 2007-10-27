#include "Dice.h"

int Dice::roll(int diceFaces)
{
	return((rand() % diceFaces) + 1);
}

int Dice::characterRoll() 
{
	//roll four six sided dice.
	setCharacterRolls(roll(6), roll(6), roll(6), roll(6));
	//find min and drop it, add the rest
	min = roll1;
	if (roll2 < min)
		min = roll2;
	else if (roll3 < min)
		min = roll3;
	else if (roll4 < min)
		min = roll4;
	return ((roll1 + roll2 + roll3 + roll4) - min);
}

void Dice::setCharacterRolls (int a, int b, int c, int d) 
{
	roll1 = a;
	roll2 = b;
	roll3 = c;
	roll4 = d;
}
