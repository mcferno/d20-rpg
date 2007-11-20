#include "EquipScreen.h"

EquipScreen::EquipScreen(int newX, int newY, int newW, int newH) : 
	Screen(newX,newY,newW,newH)
{
	background = loadImage(".\\images\\equipScreen.png");
	buttons = loadImage(".\\images\\equipButton.png");
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

	equipButton.x = x+5*16;
	equipButton.y = y+12*16;
	equipButton.clip.x = equipButton.clip.y = 0;
	equipButton.clip.w = 3*16;
	equipButton.clip.h = 16;

	unEquipButton.clip.x = equipButton.clip.x + equipButton.clip.w;
	unEquipButton.clip.y = 0;
	unEquipButton.clip.w = 5*16;
	unEquipButton.clip.h = 16;
	unEquipButton.x = x+4*16;
	unEquipButton.y = y+12*16;

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

	if(selectedEquipableItem != NULL)
	{
		if(mainCharacter->isEquipped(selectedEquipableItem))
		{
			applySurface(x+TITLE_X*16,y+TITLE_Y*16,msgTitleEquipped, screen);
			applySurface(unEquipButton.x,unEquipButton.y,buttons,screen,&unEquipButton.clip);
		}
		else
		{
			applySurface(x+TITLE_X*16,y+TITLE_Y*16,msgTitleSelected, screen);
			applySurface(equipButton.x,equipButton.y,buttons,screen,&equipButton.clip);
		}
		applySurface(x+INFO_SECTION_X*16,y+INFO_SECTION_Y*16,msgItemName,screen);
		applySurface(x+INFO_SECTION_X*16,y+(INFO_SECTION_Y+1)*16,msgWorth[0],screen);
		applySurface(x+msgWorth[0]->w+INFO_SECTION_X*16,y+(INFO_SECTION_Y+1)*16,msgItemCost,screen);
		applySurface(x+msgWorth[0]->w+msgItemCost->w+INFO_SECTION_X*16,y+(INFO_SECTION_Y+1)*16,msgWorth[1],screen);

		for(int i=0;i<NUM_CUSTOM;i++)
		{
			if(msgCustom[i] != NULL && msgItemCustom[i] != NULL)
			{
				applySurface(x+(INFO_SECTION_X)*16,y+(INFO_SECTION_Y+2+i)*16,msgCustom[i],screen);
				applySurface(x+msgCustom[i]->w+(INFO_SECTION_X)*16,y+(INFO_SECTION_Y+2+i)*16,msgItemCustom[i],screen);
			}
		}
	}

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

void EquipScreen::setSignal(bool* signal)
{
	notifyCompletion = signal;
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

	if(clickedExit(clickX, clickY))
	{
		*notifyCompletion = true;
	}
	else if(selectedEquipableItem != NULL 
		&& clickedButton(clickX, clickY, unEquipButton) 
		&& mainCharacter->isEquipped(selectedEquipableItem))
	{
		mainCharacter->unEquip(selectedEquipableItem);
		deselectItems();
	}
	else if(selectedEquipableItem != NULL && clickedButton(clickX, clickY, equipButton) )
	{
		equipItem();
	}
	else if(clickedEquiped(clickX, clickY, EQUIP_HEAD_X, EQUIP_HEAD_Y))
	{
		if(mainCharacter->equippedHelmet != NULL)
		{
			highlightInInventory(mainCharacter->equippedHelmet);
			selectedItem(mainCharacter->equippedHelmet);
		}
	}
	else if(clickedEquiped(clickX, clickY, EQUIP_BODY_X, EQUIP_BODY_Y))
	{
		if(mainCharacter->equippedVest != NULL)
		{
			highlightInInventory(mainCharacter->equippedVest);
			selectedItem(mainCharacter->equippedVest);
		}
	}
	else if(clickedEquiped(clickX, clickY, EQUIP_WEAPON_X, EQUIP_WEAPON_Y))
	{
		if(mainCharacter->equippedWeapon != NULL)
		{
			highlightInInventory(mainCharacter->equippedWeapon);
			selectedItem(mainCharacter->equippedWeapon);
		}
	}
	else if(clickedEquiped(clickX, clickY, EQUIP_SHIELD_X, EQUIP_SHIELD_Y))
	{
		if(mainCharacter->equippedShield != NULL)
		{
			highlightInInventory(mainCharacter->equippedShield);
			selectedItem(mainCharacter->equippedShield);
		}
	}
	else
	{}
}

void EquipScreen::mouseRight(int clickX, int clickY)
{
	deselectItems();
}

bool EquipScreen::clickedEquiped(int clickX, int clickY,int xLoc,int yLoc)
{
	return (clickX >= x+xLoc*16 && clickX < x+xLoc*16 + 16 && clickY >= y+yLoc*16 && clickY < y+yLoc*16 + 16);
}

void EquipScreen::highlightInInventory(Item* toHighlight)
{
	int size = mainCharacter->inventory.getSize();
	for(int i=0;i<size;i++)
	{
		if(mainCharacter->inventory.get(i) == toHighlight)
		{
			selectedInventory = i;
			return;
		}
	}
	selectedInventory = -1;
}

void EquipScreen::selectedItem(Item* item)
{
	selectedEquipableItem = (EquipableItem*)item;

	if(selectedEquipableItem != NULL)
	{
		char tempBuffer[5];
		msgItemName = TTF_RenderText_Solid(fontCalibri,selectedEquipableItem->name,fontColorWhite);
		_itoa_s(selectedEquipableItem->cost.getGold(),tempBuffer,10);
		msgItemCost = TTF_RenderText_Solid(fontCalibri,tempBuffer,fontColorWhite);

		if(selectedEquipableItem->itemType == EquipableItem::WEAPON)
		{
			Weapon* tempWeapon = (Weapon*)selectedEquipableItem;
			if(tempWeapon != NULL)
			{
				msgCustom[0] = TTF_RenderText_Solid(fontCalibri,"Damage Dice: D",fontColorWhite);
				msgCustom[1] = TTF_RenderText_Solid(fontCalibri,"Critical Multiplyer: ",fontColorWhite);

				_itoa_s(tempWeapon->getDamage(),tempBuffer,10);
				msgItemCustom[0] = TTF_RenderText_Solid(fontCalibri,tempBuffer,fontColorWhite);
				_itoa_s(tempWeapon->getCriticalMultiplier(),tempBuffer,10);
				msgItemCustom[1] = TTF_RenderText_Solid(fontCalibri,tempBuffer,fontColorWhite);

				if(tempWeapon->getRangeIncrement() > 0)
				{
					msgCustom[2] = TTF_RenderText_Solid(fontCalibri,"Range Increment: ",fontColorWhite);
					_itoa_s(tempWeapon->getRangeIncrement(),tempBuffer,10);
					msgItemCustom[2] = TTF_RenderText_Solid(fontCalibri,tempBuffer,fontColorWhite);
				}
			}
		}
		else	// item type is armor
		{
			Armor* tempArmor = (Armor*)selectedEquipableItem;
			if(tempArmor != NULL)
			{
				msgCustom[0] = TTF_RenderText_Solid(fontCalibri,"Armor Bonus: ",fontColorWhite);
				msgCustom[1] = TTF_RenderText_Solid(fontCalibri,"Max Dexterity Bonus: ",fontColorWhite);
				msgCustom[2] = TTF_RenderText_Solid(fontCalibri,"Armor Check Penalty: ",fontColorWhite);

				_itoa_s(tempArmor->getArmorBonus(),tempBuffer,10);
				msgItemCustom[0] = TTF_RenderText_Solid(fontCalibri,tempBuffer,fontColorWhite);
				_itoa_s(tempArmor->getMaxDexBonus(),tempBuffer,10);
				msgItemCustom[1] = TTF_RenderText_Solid(fontCalibri,tempBuffer,fontColorWhite);
				_itoa_s(tempArmor->getArmorCheckPenalty(),tempBuffer,10);
				msgItemCustom[2] = TTF_RenderText_Solid(fontCalibri,tempBuffer,fontColorWhite);
			}
		}
	}
}

void EquipScreen::equipItem()
{
	if(selectedEquipableItem != NULL)
	{
		switch(selectedEquipableItem->itemType)
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

bool EquipScreen::clickedButton(int clickX, int clickY, Button toCheck)
{
	// check the bounds of the button
	return (clickX >= toCheck.x && clickX < toCheck.x + toCheck.clip.w && clickY >= toCheck.y && clickY < toCheck.y + toCheck.clip.h);
}