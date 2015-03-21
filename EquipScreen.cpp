#include "EquipScreen.h"

EquipScreen::EquipScreen(int newX, int newY, int newW, int newH) :
	Screen(newX,newY,newW,newH)
{
	background = loadImage(EQUIPMENT_SCREEN_BACKDROP);
	buttons = loadImage(EQUIPMENT_SCREEN_BUTTONS);
	selectedInventory = -1;
	selectedEquipableItem = NULL;
	fontColorWhite.r = fontColorWhite.g = fontColorWhite.b = 0xFF;

	msgTitleSelected = TTF_RenderText_Solid(fontCalibriBold,"Selected Item",fontColorWhite);
	msgTitleEquipped = TTF_RenderText_Solid(fontCalibriBold,"Equipped Item",fontColorWhite);
	msgItemName = NULL;
	msgItemCost = NULL;
	msgWorth[0] = TTF_RenderText_Solid(fontCalibri,"Worth: ",fontColorWhite);
	msgWorth[1] = TTF_RenderText_Solid(fontCalibri," gold",fontColorWhite);
	msgCustom[0] = NULL;
	msgCustom[1] = NULL;
	msgCustom[2] = NULL;
	msgItemCustom[0] = NULL;
	msgItemCustom[1] = NULL;
	msgItemCustom[2] = NULL;

	equipButton = Button(x+5*TILE_SIZE,y+12*TILE_SIZE,3*TILE_SIZE,TILE_SIZE,0,0,buttons);
	unEquipButton = Button(x+4*TILE_SIZE,y+12*TILE_SIZE,5*TILE_SIZE,TILE_SIZE,3*TILE_SIZE,0,buttons);
}

void EquipScreen::paint()
{
	applySurface(x,y,background,screen);

	int size = mainCharacter->inventory.getSize();
	for(int i=0;i<size;i++)
	{
		paintItem(x+(INVENTORY_X+i)*16,y+INVENTORY_Y*16,mainCharacter->inventory.get(i));
		if(selectedInventory == i)
		{
			applySurface(x+(INVENTORY_X+i)*16,y+INVENTORY_Y*16,highlightTile,screen);
		}
	}
	if(selectedInventory != -1)
	{
		applySurface(x+INFO_SECTION_X*16,y+INFO_SECTION_Y*16,msgItemName,screen);
		applySurface(x+INFO_SECTION_X*16,y+(INFO_SECTION_Y+1)*16,msgWorth[0],screen);
		applySurface(x+msgWorth[0]->w+INFO_SECTION_X*16,y+(INFO_SECTION_Y+1)*16,msgItemCost,screen);
		applySurface(x+msgWorth[0]->w+msgItemCost->w+INFO_SECTION_X*16,y+(INFO_SECTION_Y+1)*16,msgWorth[1],screen);
	}

	for(int i=0;i<NUM_CUSTOM;i++)
	{
		if(msgCustom[i] != NULL && msgItemCustom[i] != NULL)
		{
			applySurface(x+(INFO_SECTION_X)*16,y+(INFO_SECTION_Y+2+i)*16,msgCustom[i],screen);
			applySurface(x+msgCustom[i]->w+(INFO_SECTION_X)*16,y+(INFO_SECTION_Y+2+i)*16,msgItemCustom[i],screen);
		}
	}

	// paint the selected item if it is equipable
	if(selectedEquipableItem != NULL)
	{
		if(mainCharacter->isEquipped(selectedEquipableItem))
		{
			applySurface(x+TITLE_X*16,y+TITLE_Y*16,msgTitleEquipped, screen);
			unEquipButton.paint();
		}
		else
		{
			applySurface(x+TITLE_X*16,y+TITLE_Y*16,msgTitleSelected, screen);
			equipButton.paint();
		}
	}

	// paint the graphics for any equipped items
	if(mainCharacter->equippedWeapon != NULL)
		paintItem(x+EQUIP_WEAPON_X*16,y+EQUIP_WEAPON_Y*16,mainCharacter->equippedWeapon);

	if(mainCharacter->equippedHelmet != NULL)
		paintItem(x+EQUIP_HEAD_X*16,y+EQUIP_HEAD_Y*16,mainCharacter->equippedHelmet);

	if(mainCharacter->equippedVest != NULL)
		paintItem(x+EQUIP_BODY_X*16,y+EQUIP_BODY_Y*16,mainCharacter->equippedVest);

	if(mainCharacter->equippedShield != NULL)
		paintItem(x+EQUIP_SHIELD_X*16,y+EQUIP_SHIELD_Y*16,mainCharacter->equippedShield);
}

void EquipScreen::paintItem(int xLoc, int yLoc, Item* item)
{
	applySurface(xLoc,yLoc,item->graphics->image,screen,&item->graphics->clip[item->index]);
}

void EquipScreen::deselectItems()
{
	selectedInventory = -1;
	selectedEquipableItem = NULL;

	if(msgItemName != NULL)
	{
		SDL_FreeSurface(msgItemName);
		msgItemName = NULL;
	}
	if(msgItemCost != NULL)
	{
		SDL_FreeSurface(msgItemCost);
		msgItemCost = NULL;
	}

	for(int i=0;i<NUM_CUSTOM;i++)
	{
		if(msgCustom[i] != NULL)
		{
			SDL_FreeSurface(msgCustom[i]);
			msgCustom[i] = NULL;
		}
	}

	for(int i=0;i<NUM_CUSTOM;i++)
	{
		if(msgItemCustom[i] != NULL)
		{
			SDL_FreeSurface(msgItemCustom[i]);
			msgItemCustom[i] = NULL;
		}
	}
}

void EquipScreen::userExited()
{
	deselectItems();
}

bool EquipScreen::clickedExit(int clickX, int clickY)
{
	return (clickX >= x+w-16 && clickX < x+w && clickY >= y && clickY < y+16);
}

void EquipScreen::mouseLeft(int clickX, int clickY)
{
	int numItems = mainCharacter->inventory.getSize();
	for(int i=0;i<numItems;i++)
	{
		if(clickX >= x+(i+INVENTORY_X)*16
		 && clickX <= x+(i+INVENTORY_X+1)*16
		 && clickY >= y+(INVENTORY_Y)*16
		 && clickY <= y+(INVENTORY_Y+1)*16)
		{
			selectedItem(mainCharacter->inventory.get(i));
			selectedInventory = i;
			return;
		}
	}

	// the user wished to return, signal the parent window
	if(clickedExit(clickX, clickY))
	{
		signalCompletion();
	}
	// unequip an item that was selected
	else if(selectedEquipableItem != NULL
		&& unEquipButton.inBounds(clickX, clickY)
		&& mainCharacter->isEquipped(selectedEquipableItem))
	{
		mainCharacter->unEquip(selectedEquipableItem);
		deselectItems();
	}
	else if(selectedEquipableItem != NULL && equipButton.inBounds(clickX, clickY) )
	{
		equipItem();
	}
	else if(clickedEquiped(clickX, clickY, EQUIP_HEAD_X, EQUIP_HEAD_Y))
	{
		if(mainCharacter->equippedHelmet != NULL)
		{
			int selected = findInventoryNumber(mainCharacter->equippedHelmet);
			selectedItem(mainCharacter->equippedHelmet);
			selectedInventory = selected;
		}
	}
	else if(clickedEquiped(clickX, clickY, EQUIP_BODY_X, EQUIP_BODY_Y))
	{
		if(mainCharacter->equippedVest != NULL)
		{
			int selected = findInventoryNumber(mainCharacter->equippedVest);
			selectedItem(mainCharacter->equippedVest);
			selectedInventory = selected;
		}
	}
	else if(clickedEquiped(clickX, clickY, EQUIP_WEAPON_X, EQUIP_WEAPON_Y))
	{
		if(mainCharacter->equippedWeapon != NULL)
		{
			int selected = findInventoryNumber(mainCharacter->equippedWeapon);
			selectedItem(mainCharacter->equippedWeapon);
			selectedInventory = selected;
		}
	}
	else if(clickedEquiped(clickX, clickY, EQUIP_SHIELD_X, EQUIP_SHIELD_Y))
	{
		if(mainCharacter->equippedShield != NULL)
		{
			int selected = findInventoryNumber(mainCharacter->equippedShield);
			selectedItem(mainCharacter->equippedShield);
			selectedInventory = selected;
		}
	}
}

void EquipScreen::mouseRight(int clickX, int clickY)
{
	deselectItems();
}

bool EquipScreen::clickedEquiped(int clickX, int clickY,int xLoc,int yLoc)
{
	return (clickX >= x+xLoc*16 && clickX < x+xLoc*16 + 16 && clickY >= y+yLoc*16 && clickY < y+yLoc*16 + 16);
}

int EquipScreen::findInventoryNumber(Item* toHighlight)
{
	int size = mainCharacter->inventory.getSize();

	// traverse the inventory to see if this item is present
	for(int i=0;i<size;i++)
	{
		if(mainCharacter->inventory.get(i) == toHighlight)
		{
			return i;
		}
	}
	return -1;
}

void EquipScreen::selectedItem(Item* item)
{
	// free up any residual data
	if(selectedInventory != -1)
		deselectItems();

	std::string tempBuffer;
	msgItemName = TTF_RenderText_Solid(fontCalibri,item->name,fontColorWhite);
	tempBuffer = to_string(item->cost.getGold());
	msgItemCost = TTF_RenderText_Solid(fontCalibri, tempBuffer.c_str(), fontColorWhite);

	// render any information relevant to an item with quantity, do not allow it to be equipped
	if(item->isItemOfQuantity())
	{
		msgCustom[0] = TTF_RenderText_Solid(fontCalibri,"Quantity: ",fontColorWhite);
		switch(((UsableItem*)item)->usableType)
		{
			case UsableItem::ARROW:
				tempBuffer = to_string(mainCharacter->getNumArrows());
				break;
			case UsableItem::BOLT:
				tempBuffer = to_string(mainCharacter->getNumBolts());
				break;
			default:
				tempBuffer = to_string(mainCharacter->getNumPotions());
				break;

		}
		msgItemCustom[0] = TTF_RenderText_Solid(fontCalibri, tempBuffer.c_str(), fontColorWhite);
	}
	else
		selectedEquipableItem = (EquipableItem*)item;

	if(selectedEquipableItem != NULL)
	{
		// show information specific to a weapon
		if(selectedEquipableItem->equipType == EquipableItem::WEAPON)
		{
			Weapon* tempWeapon = (Weapon*)selectedEquipableItem;
			if(tempWeapon != NULL)
			{
				msgCustom[0] = TTF_RenderText_Solid(fontCalibri,"Damage Dice: D",fontColorWhite);
				msgCustom[1] = TTF_RenderText_Solid(fontCalibri,"Critical Multiplyer: ",fontColorWhite);

				tempBuffer = to_string(tempWeapon->getDamage());
				msgItemCustom[0] = TTF_RenderText_Solid(fontCalibri, tempBuffer.c_str(), fontColorWhite);
				tempBuffer = to_string(tempWeapon->getCriticalMultiplier());
				msgItemCustom[1] = TTF_RenderText_Solid(fontCalibri, tempBuffer.c_str(), fontColorWhite);

				if(tempWeapon->getRangeIncrement() > 0)
				{
					msgCustom[2] = TTF_RenderText_Solid(fontCalibri,"Range Increment: ",fontColorWhite);
					tempBuffer = to_string(tempWeapon->getRangeIncrement());
					msgItemCustom[2] = TTF_RenderText_Solid(fontCalibri, tempBuffer.c_str(), fontColorWhite);
				}
			}
		}
		else	// item type is armor
		{
			// show information specific to armor (shield, helmet, vest)
			Armor* tempArmor = (Armor*)selectedEquipableItem;
			if(tempArmor != NULL)
			{
				msgCustom[0] = TTF_RenderText_Solid(fontCalibri,"Armor Bonus: ",fontColorWhite);
				msgCustom[1] = TTF_RenderText_Solid(fontCalibri,"Max Dexterity Bonus: ",fontColorWhite);
				msgCustom[2] = TTF_RenderText_Solid(fontCalibri,"Armor Check Penalty: ",fontColorWhite);

				tempBuffer = to_string(tempArmor->getArmorBonus());
				msgItemCustom[0] = TTF_RenderText_Solid(fontCalibri, tempBuffer.c_str(), fontColorWhite);
				tempBuffer = to_string(tempArmor->getMaxDexBonus());
				msgItemCustom[1] = TTF_RenderText_Solid(fontCalibri, tempBuffer.c_str(), fontColorWhite);
				tempBuffer = to_string(tempArmor->getArmorCheckPenalty());
				msgItemCustom[2] = TTF_RenderText_Solid(fontCalibri, tempBuffer.c_str(), fontColorWhite);
			}
		}
	}
}

void EquipScreen::equipItem()
{
	if(selectedEquipableItem != NULL)
	{
		// equip the item in the right place, depending on its type
		switch(selectedEquipableItem->equipType)
		{
			case EquipableItem::WEAPON:
				mainCharacter->equippedWeapon = (Weapon*)selectedEquipableItem;
				break;
			case EquipableItem::CHEST:
				mainCharacter->equippedVest = (Armor*)selectedEquipableItem;
				break;
			case EquipableItem::HEAD:
				mainCharacter->equippedHelmet = (Armor*)selectedEquipableItem;
				break;
			default:
				mainCharacter->equippedShield = (Armor*)selectedEquipableItem;
				break;
		}
	}
}
