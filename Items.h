#ifndef ITEMS_H
	#define ITEMS_H

#include "Graphics.h"
#include "Dice.h"
#include "Money.h"

// #####################################################################################################

/* Class Item: a generic item which has a cost when purchased.
 */
class Item
{
public:
	Item(char*,int,int);

	// how much money this item costs to obtain (if applicable)
	Money cost;

	// the name of this item, needed if the visual representation isn't distinct
	char *name;

	// tile offset for the clip
	int index;

	// the graphics to use to display this item
	static Graphics* graphics;

	// whether or not the item is unique, or has an inventory (ex: many potions, one sword)
	virtual bool isItemOfQuantity();
};

// #####################################################################################################

/* Class UsableItem: a generic item which can be used (consumed). For example, this includes
 *   potions, arrows, bolts.
 */
class UsableItem : public Item
{
protected:
	// how many of this item is there?
	int num;
public:
	// which type of consumable is this? will be needed for weapons that require consumables
	enum UsableItemType { ARROW, BOLT, POTION, NONE };
	UsableItemType usableType;

	UsableItem(char*, int, int, UsableItemType, int = 0);
	virtual bool isItemOfQuantity();

	// add more to this item's quantity
	void add(int);

	// is there any items left of this kind?
	bool isEmpty();

	// return the number of items left
	int numLeft();

	// consume one item
	void useOne();
};

// #####################################################################################################

/* Class Potion: a kind of usable item which can be taken to heal a character's
 *   hit points.
 */
class Potion : public UsableItem
{
private:
	// how much HP does this potion restore?
	int hpBonus;
public:
	Potion(char*,int,int,int);
	int getHpBonus();
};

// #####################################################################################################

/* Class UsableItemFactory: Creates a finite number of items which will be
 *   available across the game. Only one set of finite items will exist
 *   and it is this class' responsibility to pass those items along.
 */
class UsableItemFactory
{
private:
	// the only set of items for the majority of the game
	static UsableItem* availableUsableItems[];
	static int numAvailableUsableItems;
	static void loadGraphics();
public:
	static UsableItem** getAllUsableItems();
	static int getNumUsableItems();
};

// #####################################################################################################

/* Class EquipableItem: a type of item which can be worn, typically for battle
 */
class EquipableItem : public Item
{
public:
	// to identify the difference between equipable items
	enum EquipableItemType { HEAD, CHEST, WEAPON, SHIELD };
	EquipableItemType equipType;
	EquipableItem(char*,int,int,EquipableItemType);
};

// #####################################################################################################

/* Class Armor: a type of equippable item which provides defences for the character wearing it
 */
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

// #####################################################################################################

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

// #####################################################################################################

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
	UsableItem::UsableItemType requiredConsumable;
	// name, damage dice type, critical multiplier, cost in gold, graphicOffset
	Weapon(char*,int,int,int,int);

	// name, damage dice type, critical multiplier, cost in gold, graphicOffset, range, requiredItem
	Weapon(char*,int,int,int,int,int,UsableItem::UsableItemType);

	char* getName();
	int getDamage();
	int getRangeIncrement();
	int getCriticalMultiplier();
};

// #####################################################################################################

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

// #####################################################################################################

#endif