#include "Dice.h"

//Input takes the type of dice and the amount of times it should be rolled
//Output gives you the total of the dice rolls
int Dice::roll(int diceFaces, int rollTimes)
{
	int rollSum = 0;
	if(rollTimes > 0)
	{
		for(int i=0;i<rollTimes;i++)
			rollSum += ((rand() % diceFaces) + 1);
	}
	return rollSum;
}

