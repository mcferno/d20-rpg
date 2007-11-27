#ifndef EQUIPSCREEN_H
	#define EQUIPSCREEN_H

#include "Screen.h"

/* Class EquipScreen: a screen which allows the user to see which items he/she is 
 *   carrying, and if the item is wearable, it allows the user to equip the item.
 */
class EquipScreen : public Screen
{
private:
	SDL_Surface *background;
	SDL_Surface *buttons;

	// buttons to manage equipping and unequipping items
	Button equipButton;
	Button unEquipButton;

	// rendered informative text
	SDL_Surface *msgTitleSelected;
	SDL_Surface *msgTitleEquipped;
	SDL_Surface *msgItemName, *msgItemCost;
	SDL_Surface *msgWorth[2];
	static const int NUM_CUSTOM = 3;
	SDL_Surface *msgCustom[NUM_CUSTOM];
	SDL_Surface *msgItemCustom[NUM_CUSTOM];
	SDL_Color fontColorWhite;

	// the item the user has selected
	int selectedInventory;
	EquipableItem *selectedEquipableItem;

	// coordinates of varies regions, user to paint in the appropriate places
	static const int INVENTORY_X = 2;
	static const int INVENTORY_Y = 18;
	static const int INFO_SECTION_X = 2;
	static const int INFO_SECTION_Y = 5;
	static const int TITLE_X = 2;
	static const int TITLE_Y = 3;
	static const int EQUIP_HEAD_X = 17;
	static const int EQUIP_HEAD_Y = 2;
	static const int EQUIP_BODY_X = 17;
	static const int EQUIP_BODY_Y = 5;
	static const int EQUIP_WEAPON_X = 15;
	static const int EQUIP_WEAPON_Y = 9;
	static const int EQUIP_SHIELD_X = 19;
	static const int EQUIP_SHIELD_Y = 9;

		// match a pointer to an inventory location
	int findInventoryNumber(Item*);

	void paintItem(int, int, Item*);
	void selectedItem(Item*);
	void equipItem();
	void deselectItems();
	bool clickedEquiped(int,int,int,int);
	bool clickedExit(int,int);
public:
	EquipScreen(int, int, int, int);
	void mouseLeft(int,int);
	void mouseRight(int,int);
	void paint();
	void userExited();
};

#endif