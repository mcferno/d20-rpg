#ifndef EQUIPSCREEN_H
	#define EQUIPSCREEN_H

#include "Screens.h"

class EquipScreen : public Screen
{
private:
	SDL_Surface *background;
	SDL_Surface *buttons;
	Button equipButton;
	Button unEquipButton;

	SDL_Surface *msgTitleSelected;
	SDL_Surface *msgTitleEquipped;
	SDL_Surface *msgItemName, *msgItemCost;
	SDL_Surface *msgWorth[2];
	static const int NUM_CUSTOM = 3;
	SDL_Surface *msgCustom[NUM_CUSTOM];
	SDL_Surface *msgItemCustom[NUM_CUSTOM];
	SDL_Color fontColorWhite;

	int selectedInventory;
	EquipableItem *selectedEquipableItem;

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

	void paintItem(int, int, Item*);
	void selectedItem(Item*);
	void equipItem();
	void deselectItems();
	int findInventoryNumber(Item*);
	bool clickedButton(int,int,Button);
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