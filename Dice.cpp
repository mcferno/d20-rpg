#include "Dice.h"

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

