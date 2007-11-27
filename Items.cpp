#include "Items.h"


Graphics* Item::graphics = NULL;

//#####################################################################################################

Item::Item(char* newName,int newCostInGold, int graphicOffset)
{
	name = newName;
	if(newCostInGold > 0)
		cost.addGold(newCostInGold);
	index = graphicOffset;
}
bool Item::isItemOfQuantity()
{
	return false;
}

//#####################################################################################################

EquipableItem::EquipableItem(char* newName,int newCostInGold,int graphicOffset,EquipableItemType newEquipType) :
	Item(newName,newCostInGold,graphicOffset)
{
	equipType = newEquipType;
}


//#####################################################################################################

UsableItem::UsableItem(char* newName, int newCostInGold, int graphicOffset, UsableItemType newUsableType, int newNum) :
	Item(newName,newCostInGold,graphicOffset)
{
	usableType = newUsableType;
	num = newNum;
}

void UsableItem::add(int newAmount)
{
	if(newAmount > 0)
		num += newAmount;
}
bool UsableItem::isEmpty()
{
	return (num == 0);
}
int UsableItem::numLeft()
{
	return num;
}
void UsableItem::useOne()
{
	if(!isEmpty())
		num--;
}

bool UsableItem::isItemOfQuantity()
{
	return true;
}

//#####################################################################################################

Potion::Potion(char* newName,int newCostInGold, int graphicOffset, int newHpBonus) :
	UsableItem(newName,newCostInGold,graphicOffset,UsableItem::POTION,1)
{
	hpBonus = (newHpBonus>0)?newHpBonus:0;
}

int Potion::getHpBonus()
{
	if(hpBonus > 0)
		return hpBonus;
	return 0;
}

//#####################################################################################################

// statically create the set of all available usable items in the game
UsableItem* UsableItemFactory::availableUsableItems[] = { 
	new UsableItem("Arrow",1,59,UsableItem::ARROW,20),
	new UsableItem("Bolt",1,57,UsableItem::BOLT,10),
	new Potion("Potion",3,60,5)
};

// must match the number of elements created above
int UsableItemFactory::numAvailableUsableItems = 3;

void UsableItemFactory::loadGraphics()
{
	Item::graphics = new Graphics(ITEM_GRAPHICS,0xFF,0x0,0xFF);
}

UsableItem** UsableItemFactory::getAllUsableItems()
{
	if(Item::graphics == NULL)
		loadGraphics();
	return availableUsableItems;
}

int UsableItemFactory::getNumUsableItems()
{
	return numAvailableUsableItems;
}

//#####################################################################################################

Weapon::Weapon(char* newName, int newDamage,int newCritical, int newCostInGold, int graphicOffset) : 
	DAMAGE_DICE_TYPE(newDamage), 
	CRITICAL_MULTIPLIER(newCritical),
	RANGE_INCREMENT(0),
	EquipableItem(newName,newCostInGold,graphicOffset,EquipableItem::WEAPON)
{
	requiredConsumable = UsableItem::NONE;
}

Weapon::Weapon(char* newName, int newDamage,int newCritical, int newCostInGold, int graphicOffset, int newRange, UsableItem::UsableItemType newRequiredConsumable) : 
	DAMAGE_DICE_TYPE(newDamage), 
	CRITICAL_MULTIPLIER(newCritical),
	RANGE_INCREMENT(newRange/FT_TO_TILES),
	EquipableItem(newName,newCostInGold,graphicOffset,EquipableItem::WEAPON)
{
	requiredConsumable = newRequiredConsumable;
}


int Weapon::getDamage()
{
	return DAMAGE_DICE_TYPE;
}

char* Weapon::getName()
{
	return name;
}

int Weapon::getRangeIncrement()
{
	return RANGE_INCREMENT;
}

int Weapon::getCriticalMultiplier()
{
	return CRITICAL_MULTIPLIER;
}

//#####################################################################################################

Armor::Armor(char* newName, int newBonus,int newMaxDex, int newCheckPenalty, int newCostInGold, int graphicOffset, EquipableItemType newEquipableType) : 
	ARMOR_BONUS(newBonus),
	MAX_DEX_BONUS(newMaxDex),
	ARMOR_CHECK_PENALTY(newCheckPenalty),
	EquipableItem(newName,newCostInGold,graphicOffset,newEquipableType)
{
}

int Armor::getArmorBonus()
{
	return ARMOR_BONUS;
}
int Armor::getMaxDexBonus()
{
	return MAX_DEX_BONUS;
}
int Armor::getArmorCheckPenalty()
{
	return ARMOR_CHECK_PENALTY;
}

//#####################################################################################################

// statically create the set of all available weapons in the game
// name, damage dice type, critical multiplier, cost in gold, graphicOffset
// name, damage dice type, critical multiplier, cost in gold, graphicOffset, range, WeaponConsumable
Weapon* WeaponFactory::availableWeapons[] = { 
	new Weapon("Small Short Sword",Dice::D4,2,10,4),
	new Weapon("Medium Short Sword",Dice::D6,2,10,26),
	new Weapon("Longbow",Dice::D6,3,75,58,100,UsableItem::ARROW),
	new Weapon("Crossbow",Dice::D4,2,100,46,30,UsableItem::BOLT)
};

// must match the number of elements created above
int WeaponFactory::numAvailableWeapons = 4;

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
	new Armor("Padded Light Armor",1,8,0,5,0,EquipableItem::CHEST),
	new Armor("Scale mail",4,3,-4,50,33,EquipableItem::CHEST),
	new Armor("Splint Mail",6,0,-7,200,22,EquipableItem::CHEST),
	new Armor("Basic Helm",1,0,0,10,3,EquipableItem::HEAD),
	new Armor("Buckler",1,0,-1,15,1,EquipableItem::SHIELD),
	new Armor("Heavy Steel Shield",2,0,-2,20,23,EquipableItem::SHIELD)
};

// must match the number of elements created above
int ArmorFactory::numAvailableArmor = 6;

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