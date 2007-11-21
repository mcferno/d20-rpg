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
	Item(char*,int,int);
	virtual bool isItemOfQuantity();
};

class UsableItem : public Item
{
protected:
	int num;
public:
	enum UsableItemType { ARROW, BOLT, POTION, NONE };
	UsableItemType usableType;

	UsableItem(char*, int, int, UsableItemType, int = 0);

	void add(int);
	bool isEmpty();
	int numLeft();
	void useOne();
	virtual bool isItemOfQuantity();
};

class Potion : public UsableItem
{
private:
	int hpBonus;
public:
	Potion(char*,int,int,int);
};

/* Class UsableItemFactory: Creates a finite number of items which will be
 *   available across the game. Only one set of finite items will exist
 *   and it is this class' responsibility to pass those items along.
 */
class UsableItemFactory
{
private:
	static UsableItem* availableUsableItems[];
	static int numAvailableUsableItems;
	static void loadGraphics();
public:
	static UsableItem** getAllUsableItems();
	static int getNumUsableItems();
};


class EquipableItem : public Item
{
public:
	// to identify the difference between equipable items
	enum EquipableItemType { HEAD, CHEST, WEAPON, SHIELD };
	EquipableItemType equipType;
	EquipableItem(char*,int,int,EquipableItemType);
};

class Armor : public EquipableItem
{
private:
	const int ARMOR_BONUS;
	const int MAX_DEX_BONUS;
	const int ARMOR_CHECK_PENALTY;
public:
	// name, armor bonus, max dex bonus, armor check penalty, cost in gold, graphicOffset, itemType
	Armor(char*,int,int,int,int,int,EquipableItemType);
	int getArmorBonus();
	int getMaxDexBonus();
	int getArmorCheckPenalty();
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

	UsableItem::UsableItemType requiredConsumable;
public:
	// name, damage dice type, critical multiplier, cost in gold, graphicOffset
	Weapon(char*,int,int,int,int);

	// name, damage dice type, critical multiplier, cost in gold, graphicOffset, range, requiredItem
	Weapon(char*,int,int,int,int,int,UsableItem::UsableItemType);
	
	int getDamage();
	int getRangeIncrement();
	int getCriticalMultiplier();
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