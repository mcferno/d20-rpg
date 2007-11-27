#include "Money.h"

Money::Money(int copper, int silver, int gold, int platinum)
{
	// store all initial coins in their copper equivalent
	totalCopper = copper + (silver * SILVER_TO_COPPER) + (gold * GOLD_TO_COPPER) + (platinum * PLATINUM_TO_COPPER);
}

void Money::addCoin(int amount, int coinType)
{
	// add the appropriate type of coin
	switch(coinType)
	{
		case COPPER:
			addCopper(amount);
			break;
		case SILVER:
			addSilver(amount);
			break;
		case GOLD:
			addGold(amount);
			break;
		case PLATINUM:
			addPlatinum(amount);
	}
}

void Money::addCopper(int copper)
{
	// no restriction on negative coin, since this function is reused by 'remove'
	totalCopper += copper;

	if(totalCopper < 0)
		totalCopper = 0;
}

void Money::addSilver(int silver)
{
	if(silver > 0)
		addCopper(silver*SILVER_TO_COPPER);
}

void Money::addGold(int gold)
{
	if(gold > 0)
		addCopper(gold*GOLD_TO_COPPER);
}

void Money::addPlatinum(int platinum)
{
	if(platinum > 0)
		addCopper(platinum*PLATINUM_TO_COPPER);
}

// remove coin functions make reuse of the add function
void Money::removeCopper(int copper)
{
	if(copper > 0)
		addCopper(-copper);
}

void Money::removeSilver(int silver)
{
	if(silver > 0)
		addCopper(-silver*SILVER_TO_COPPER);
}

void Money::removeGold(int gold)
{
	if(gold > 0)
		addCopper(-gold*GOLD_TO_COPPER);
}

void Money::removePlatinum(int platinum)
{
	if(platinum > 0)
		addCopper(-platinum*PLATINUM_TO_COPPER);
}

int Money::getCopper()
{
	return totalCopper;
}

int Money::getSilver()
{
	return totalCopper / SILVER_TO_COPPER;
}

int Money::getGold()
{
	return totalCopper / GOLD_TO_COPPER;
}

int Money::getPlatinum()
{
	return totalCopper / PLATINUM_TO_COPPER;
}

bool Money::operator>(const Money &rhs)
{
	return (this->totalCopper > rhs.totalCopper);
}

bool Money::operator>=(const Money &rhs)
{
	return (this->totalCopper >= rhs.totalCopper);
}

bool Money::operator<(const Money &rhs)
{
	return (this->totalCopper < rhs.totalCopper);
}

bool Money::operator<=(const Money &rhs)
{
	return (this->totalCopper <= rhs.totalCopper);
}

bool Money::operator==(const Money &rhs)
{
	return (this->totalCopper == rhs.totalCopper);
}

Money & Money::operator+=(const Money &rhs)
{
	this->addCopper(rhs.totalCopper);
	return *this;
}

Money & Money::operator-=(const Money &rhs)
{
	this->removeCopper(rhs.totalCopper);
	return *this;
}
