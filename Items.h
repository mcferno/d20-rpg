#ifndef ITEMS_H
	#define ITEMS_H

#include "Graphics.h"
#include "Dice.h"
#include "Money.h"

static char* ITEM_GRAPHICS = ".\\images\\items.bmp";

class Item
{
public:
	Money cost;
	char *name;
	// tile offset for the clip
	int index;
	static Graphics* graphics;
};

class UsableItem : public Item
{
};

class EquipableItem : public Item
{
public:
	// to identify the difference between equipable items
	enum ItemTypes { HEAD, CHEST, WEAPON, SHIELD };
	ItemTypes itemType;
};

class Armor : public EquipableItem
{
private:
	int armorBonus;
	int maxDexBonus;
	int armorCheckPenalty;
};

class Weapon : public EquipableItem
{
protected:
	const int DAMAGE_DICE_TYPE;
	const int RANGE_INCREMENT;
	const int CRITICAL_MULTIPLIER;
public:
	// name, damage dice type, critical multiplier, cost in gold, graphicOffset, range (optional)
	Weapon(char*,int,int,int,int,int = 0);
	int getDamage();
	int getRangeIncrement();
};

class WeaponFactory
{
private:
	static Weapon availableWeapons[];
	static void loadGraphics();
public:
	static Weapon* getAllWeapons();
};

#endif