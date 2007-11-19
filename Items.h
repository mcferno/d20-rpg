#ifndef ITEMS_H
	#define ITEMS_H

#include "Graphics.h"
#include "Dice.h"
#include "Money.h"

static char* ITEM_GRAPHICS = ".\\images\\items.png";

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
	const int ARMOR_BONUS;
	const int MAX_DEX_BONUS;
	const int ARMOR_CHECK_PENALTY;
public:
	// name, armor bonus, max dex bonus, armor check penalty, cost in gold, graphicOffset, itemType
	Armor(char*,int,int,int,int,int,ItemTypes);
};

/* Class ArmorFactory: Creates a finite number of armor which will be
 *   available across the game. Only one set of finite armor will exist
 *   and it is this class' responsibility to pass those armor along.
 */
class ArmorFactory
{
private:
	static Armor* availableArmor[];
	static int numAvailableArmor;
	static void loadGraphics();
public:
	static Armor** getAllArmor();
	static int getNumArmor();
};


/* Class Weapon: Any kind of weapon (both range weapon and melee weapon). A
 *   set of weapons will be precreated and will be used across the game
 */
class Weapon : public EquipableItem
{
private:
	const int DAMAGE_DICE_TYPE;
	const int RANGE_INCREMENT;
	const int CRITICAL_MULTIPLIER;
public:
	// name, damage dice type, critical multiplier, cost in gold, graphicOffset, range (optional)
	Weapon(char*,int,int,int,int,int = 0);
	int getDamage();
	int getRangeIncrement();
};

/* Class WeaponFactory: Creates a finite number of weapons which will be
 *   available across the game. Only one set of finite weapons will exist
 *   and it is this class' responsibility to pass those weapons along.
 */
class WeaponFactory
{
private:
	static Weapon* availableWeapons[];
	static int numAvailableWeapons;
	static void loadGraphics();
public:
	static Weapon** getAllWeapons();
	static int getNumWeapons();
};

#endif