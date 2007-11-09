#include "Items.h"


Graphics* Item::graphics = NULL;

//#####################################################################################################

Weapon::Weapon(char* newName, int newDamage,int newCritical, int newCostInGold, int graphicOffset, int newRange) : 
	DAMAGE_DICE_TYPE(newDamage), 
	CRITICAL_MULTIPLIER(newCritical),
	RANGE_INCREMENT(newRange)
{
	name = newName;
	cost.addGold(newCostInGold);
	itemType = WEAPON;

	index = graphicOffset;
}

//#####################################################################################################

// statically create the set of all available weapons in the game
Weapon WeaponFactory::availableWeapons[] = { 
	Weapon("Small Short Sword",Dice::D4,2,10,12),
	Weapon("Medium Short Sword",Dice::D6,2,10,13)
};

void WeaponFactory::loadGraphics()
{
	Item::graphics = new Graphics(ITEM_GRAPHICS,0xFF,0x0,0xFF);
}

Weapon* WeaponFactory::getAllWeapons()
{
	if(Item::graphics == NULL)
		loadGraphics();
	return availableWeapons;
}