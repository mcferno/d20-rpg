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

Armor::Armor(char* newName, int newBonus,int newMaxDex, int newCheckPenalty, int newCostInGold, int graphicOffset, ItemTypes newType) : 
	ARMOR_BONUS(newBonus),
	MAX_DEX_BONUS(newMaxDex),
	ARMOR_CHECK_PENALTY(newCheckPenalty)
{
	name = newName;
	cost.addGold(newCostInGold);
	itemType = newType;

	index = graphicOffset;
}

//#####################################################################################################

// statically create the set of all available weapons in the game
// (char* newName, int newDamage,int newCritical, int newCostInGold, int graphicOffset, int newRange)
Weapon* WeaponFactory::availableWeapons[] = { 
	new Weapon("Small Short Sword",Dice::D4,2,10,4),
	new Weapon("Medium Short Sword",Dice::D6,2,10,26)
};

// must match the number of elements created above
int WeaponFactory::numAvailableWeapons = 2;

void WeaponFactory::loadGraphics()
{
	Item::graphics = new Graphics(ITEM_GRAPHICS,0xFF,0x0,0xFF);
}

Weapon** WeaponFactory::getAllWeapons()
{
	if(Item::graphics == NULL)
		loadGraphics();
	return availableWeapons;
}

int WeaponFactory::getNumWeapons()
{
	return numAvailableWeapons;
}

//#####################################################################################################

// statically create the set of all available weapons in the game
// (char* newName, int newBonus,int newMaxDex, int newCheckPenalty, int newCostInGold, int graphicOffset, itemType
Armor* ArmorFactory::availableArmor[] = { 
	new Armor("Padded Light Armor",1,8,0,5,33,EquipableItem::CHEST),
	new Armor("Basic Helm",1,0,0,10,3,EquipableItem::HEAD),
	new Armor("Buckler",1,0,-1,15,1,EquipableItem::SHIELD)
};

// must match the number of elements created above
int ArmorFactory::numAvailableArmor = 3;

void ArmorFactory::loadGraphics()
{
	Item::graphics = new Graphics(ITEM_GRAPHICS,0xFF,0x0,0xFF);
}

Armor** ArmorFactory::getAllArmor()
{
	if(Item::graphics == NULL)
		loadGraphics();
	return availableArmor;
}

int ArmorFactory::getNumArmor()
{
	return numAvailableArmor;
}