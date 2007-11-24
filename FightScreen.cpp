#include "FightScreen.h"




FightScreen::FightScreen(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{

	//temp for nowthisMo
	
	
	//Graphics* monsterGraphics = new Graphics(".\\images\\enemies.png",0xFF, 0x0, 0xFF);
	//thisMonster = new Monster(15,20,6,2,monsterGraphics,Monster::SKELETON,2);

	//playerHp = mainCharacter->getHp();
	//monsterHp = thisMonster->getHp();

	//lastPlayerRoll = 0;
	//lastMonsterRoll = 0;
	
	//attackRoll = false;
	//initiativeRoll = true;
	//attacked = false;
	//hasRolledInitiative = false;
	//hasGeneratedStats = false;
	//finished = false;
	damageRoll = false;

	//playerInitiative = 0;
	//monsterInitiative = 0;

	//background = loadImage(".\\images\\fightScreen.png");
	//buttons = loadImage(".\\images\\fightButtons.png");

	//notifyCompletion = NULL;

	//// initialize the buttons used to buy and sell
	//rollButton.clip.x = 0;
	//rollButton.clip.y = attackButton.clip.y = inventoryButton.clip.y = runButton.clip.y = 0;
	//attackButton.clip.x = 5*16;
	//inventoryButton.clip.x=10*16;
	//runButton.clip.x=15*16;
	//rollButton.clip.w = attackButton.clip.w = inventoryButton.clip.w = runButton.clip.w = 5*16;
	//rollButton.clip.h = attackButton.clip.h = inventoryButton.clip.h = runButton.clip.h = 2*16;


	//rollButton.x = attackButton.x = x+(BUTTON_SECTION_X);
	//rollButton.y = attackButton.y = y+(BUTTON_SECTION_Y);

	//inventoryButton.x = x+(5*16)+(BUTTON_SECTION_X);
	//inventoryButton.y = y+(BUTTON_SECTION_Y);

	//runButton.x = x+(10*16)+(BUTTON_SECTION_X);
	//runButton.y = y+(BUTTON_SECTION_Y);

	//fontColorWhite.r = fontColorWhite.g = fontColorWhite.b = 0xFF;
	//fontColorRed.r = 0xFF;
	//fontColorRed.g = fontColorRed.b = 0x00;


	//msgInitiativeRoll = NULL;
	//msgAttackRoll = NULL;
	//msgStatic = NULL;
	//msgDynamic = NULL;
}

void FightScreen::paint()
{
//	applySurface(x,y,background,screen);
//	//paint buttons
//	if(initiativeRoll)
//	{
//		msgDynamic = TTF_RenderText_Solid(fontCalibriBold, "Roll Initiative!", fontColorRed );
//		applySurface( (x+BUTTON_SECTION_X+4)+3*16, (y+BUTTON_SECTION_Y)-2*16, msgDynamic, screen );
//		applySurface(rollButton.x, rollButton.y, buttons, screen, &rollButton.clip);
//	}
//	else
//	{
//		if (attacked) {
//			msgDynamic = TTF_RenderText_Solid(fontCalibriBold, "Your last hit inflicted:      Damage", fontColorRed );
//			applySurface( (x+BUTTON_SECTION_X+4), (y+BUTTON_SECTION_Y)-3*16, msgDynamic, screen );
//			msgDynamic = TTF_RenderText_Solid(fontCalibriBold, "His last hit inflicted:         Damage", fontColorRed );
//			applySurface( (x+BUTTON_SECTION_X+4), (y+BUTTON_SECTION_Y)-2*16, msgDynamic, screen );
//
//			_itoa_s(lastPlayerRoll,buffer,10);
//			msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
//			applySurface( (x+BUTTON_SECTION_X+4)+9.5*16, (y+BUTTON_SECTION_Y)-3*16, msgDynamic, screen );
//			_itoa_s(lastMonsterRoll,buffer,10);
//			msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
//			applySurface( (x+BUTTON_SECTION_X+4)+9.5*16, (y+BUTTON_SECTION_Y)-2*16, msgDynamic, screen );
//		}
//		applySurface(attackButton.x, attackButton.y, buttons, screen, &attackButton.clip);
//	}
//	applySurface(inventoryButton.x, inventoryButton.y, buttons, screen, &inventoryButton.clip);
//	applySurface(runButton.x, runButton.y, buttons, screen, &runButton.clip);	
//
//
//	paintStaticMessage();
//	paintDynamicMessage();
//}
//
//void FightScreen::paintStaticMessage()
//{
//
//	int xi = 3.5*16;
//	int yi = 1.5*16;
//
//	//ENTER PLAYERS NAME
//	switch (mainCharacter->getRace())
//	{
//		case 0:
//			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "HUMAN", fontColorWhite );
//			applySurface( (x+xi), ((y+yi)), msgStatic, screen );
//			break;
//		case 1:
//			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "DWARF", fontColorWhite );
//			applySurface( (x+xi), ((y+yi)), msgStatic, screen );
//			break;
//		case 2:
//			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "ELF", fontColorWhite );
//			applySurface( (x+xi), ((y+yi)), msgStatic, screen );
//			break;
//		case 3:
//			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "GNOME", fontColorWhite );
//			applySurface( (x+xi), (y+yi), msgStatic, screen );
//			break;
//	}
//	xi = xi+(4*16);
//
//	switch ((mainCharacter->getClass()))
//	{
//	case 0:
//			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "FIGHTER", fontColorWhite );
//			applySurface( (x+xi), (y+yi), msgStatic, screen );
//			break;
//	case 1:
//			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "RANGER", fontColorWhite );
//			applySurface( (x+xi), (y+yi), msgStatic, screen );
//			break;
//	}
//	xi = xi-(4*16);
//
//	//ENTER PLAYER HP
//	yi = yi+(2*16);
//	msgStatic = TTF_RenderText_Solid(fontCalibriBold, "HP:", fontColorRed );
//	applySurface( (x+xi), (y+yi), msgStatic, screen );
//	
//	yi = yi-(2*16);
//
//	//ENTER BADGUY NAME
//	xi = xi+25*16;
//	char* temp = thisMonster->getName();
//	msgStatic = TTF_RenderText_Solid(fontCalibriBold, temp , fontColorWhite );
//	applySurface( (x+xi), (y+yi), msgStatic, screen );
//
//	//ENTER MONSTER HP
//	yi = yi+2*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriBold, "HP:", fontColorRed );
//	applySurface( (x+xi), (y+yi), msgStatic, screen );
//
//	//PAINT BOTH STATS
//	paintStats();
//
//
//}
//
//void FightScreen::paintDynamicMessage()
//{
//	int xi = 0.5*16;
//	int yi = 10.5*16;
//	
//	msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, "YOUR ROLL:", fontColorWhite );
//	applySurface( (INFO_SECTION_X+x+xi), (INFO_SECTION_Y+y+yi), msgDynamic, screen );
//	_itoa_s(lastPlayerRoll,buffer,10);
//	msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
//	applySurface( (INFO_SECTION_X+x+xi+16*4), (INFO_SECTION_Y+y+yi+16.5), msgDynamic, screen );
//
//	xi = 5.5*16;
//
//	msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, "HIS ROLL:", fontColorWhite );
//	applySurface( (MONSTER_SECTION_X+x+xi), (MONSTER_SECTION_Y+y+yi), msgDynamic, screen );
//	_itoa_s(lastMonsterRoll,buffer,10);
//	msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
//	applySurface( (MONSTER_SECTION_X+x+xi), (MONSTER_SECTION_Y+y+yi+16.5), msgDynamic, screen );
//	
//	//DYNAMIC PLAYER HP
//	_itoa_s(playerHp,buffer,10);
//	msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
//	applySurface( (x+xi), (y+yi-16*7), msgDynamic, screen );
//	//DYNAMIC MONSTER HP
//	_itoa_s(monsterHp,buffer,10);
//	msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
//	applySurface( (x+xi)+25*16, (y+yi-16*7), msgDynamic, screen );
//
//	//DYNAMIC INITIATIVE PLAYER ROLL
//	_itoa_s(playerInitiative,buffer,10);
//	msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, buffer, fontColorWhite );
//	applySurface( (INFO_SECTION_X+x+8*16), (INFO_SECTION_Y+y+5*16), msgDynamic, screen );
//
//	//DYNAMIC INITIATIVE MONSTER ROLL
//	_itoa_s(monsterInitiative,buffer,10);
//	msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, buffer, fontColorWhite );
//	applySurface( (MONSTER_SECTION_X+x+8*16), (MONSTER_SECTION_Y+y+5*16), msgDynamic, screen );
//
//}
//
//
//void FightScreen::paintStats()
//{
//	int infox, infoy;
//	int xi, yi;
//
//	if (!hasGeneratedStats)
//	{
//		std::cout << "\nPLAYER STATS GENERATION: \n";
//		level = mainCharacter->getLevel();
//		ac = mainCharacter->getAC();
//		ackbonus = mainCharacter->getAttackBonus();
//		if (mainCharacter->equippedWeapon != NULL)
//			weaponName = mainCharacter->equippedWeapon->getName();
//		else
//			weaponName = "Hands";
//		if (mainCharacter->equippedWeapon != NULL)
//			weapondmg = mainCharacter->equippedWeapon->getDamage();
//		else
//			weapondmg = 3;
//
//		std::cout << "\nMONSTER STATS GENERATION: \n";
//		mlevel = thisMonster->getLevel();
//		mac = thisMonster->getAC();
//		mackbonus = thisMonster->getAttackBonus();
//		mweaponName = "Hands";
//		mweapondmg = thisMonster->getDamageDiceType();
//
//		hasGeneratedStats = true;
//	}
//
//	infox = INFO_SECTION_X;
//	infoy = INFO_SECTION_Y;
//	xi = 1;
//	yi = 1;
//
//	//DRAW HUMAN STATS
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "Level ::", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//	_itoa_s(level,buffer,10);
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, buffer, fontColorWhite );
//	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );
//	
//	yi = yi+1*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "AC ::", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//	_itoa_s(ac,buffer,10);
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, buffer, fontColorWhite );
//	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );
//
//	yi = yi+1*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "Atck Bonus ::", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//	_itoa_s(ackbonus,buffer,10);
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, buffer, fontColorWhite );
//	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );
//
//	yi = yi+1*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "Weapon ::", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, weaponName, fontColorWhite );
//	applySurface( (infox+x+xi+3.5*16), (infoy+y+yi), msgStatic, screen );
//
//	yi = yi+1*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "Weapon Dmg ::           1d", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//	_itoa_s(weapondmg,buffer,10);
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, buffer, fontColorWhite );
//	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );
//
//	yi = yi+1*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "Initiative Roll ::", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//
//	//DRAW MONSTER STATS
//	infox = MONSTER_SECTION_X;
//	infoy = MONSTER_SECTION_Y;
//	yi = 1;
//	xi = 1;
//
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "Level ::", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//	_itoa_s(mlevel,buffer,10);
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, buffer, fontColorWhite );
//	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );
//	
//	yi = yi+1*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "AC ::", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//	_itoa_s(mac,buffer,10);
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, buffer, fontColorWhite );
//	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );
//
//	yi = yi+1*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "Atck Bonus ::", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//	_itoa_s(mackbonus,buffer,10);
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, buffer, fontColorWhite );
//	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );
//
//	yi = yi+1*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "Weapon ::", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, mweaponName, fontColorWhite );
//	applySurface( (infox+x+xi+3.5*16), (infoy+y+yi), msgStatic, screen );
//
//	yi = yi+1*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "Weapon Dmg ::           1d", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
//	_itoa_s(mweapondmg,buffer,10);
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, buffer, fontColorWhite );
//	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );
//
//	yi = yi+1*16;
//	msgStatic = TTF_RenderText_Solid(fontCalibriTiny, "Initiative Roll ::", fontColorWhite );
//	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );

}

void FightScreen::mouseLeft(int clickX, int clickY)
{
	//if (hasRolledInitiative) 
	//{
	//	if (clickedButton(clickX, clickY, attackButton))
	//	{
	//		if (!finished)
	//		{
	//			playerFight();
	//			monsterFight();
	//			if (playerHp <= 0) {
	//				std::cout << "Your dead\n\n";
	//				finished = true;
	//			}
	//			if (monsterHp <= 0) {
	//				std::cout << "You Killed Him\n\n";
	//				finished = true;
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	if (clickedButton(clickX, clickY, rollButton))
	//	{
	//		lastPlayerRoll = playerInitiative = mainCharacter->getInitiativeRoll();
	//		lastMonsterRoll = monsterInitiative = thisMonster->getInitiativeRoll();
	//		hasRolledInitiative = true;
	//		initiativeRoll = false;
	//		attackRoll = true;

	//		if (monsterInitiative > playerInitiative)
	//		{
	//			std::cout << "MONSTER ATTACKS FIRST\n\n";
	//			monsterFight();
	//			paint();
	//		}
	//		else
	//			std::cout << "PLAYER ATTACKS FIRST\n\n";
	//	}
	//}

	//if (clickedButton(x+5+BUTTON_SECTION_X, y+BUTTON_SECTION_Y, inventoryButton))
	//{
	//	//SHOW INVENTORY 
	//}

	//if (clickedButton(x+10+BUTTON_SECTION_X, y+BUTTON_SECTION_Y, runButton))
	//{
	//	//EXIT O DEAR
	//}
}


void FightScreen::mouseRight(int clickX, int clickY)
{
}

bool FightScreen::clickedButton(int clickX, int clickY, Button toCheck)
{
	//check the bounds of the button
	return (clickX >= toCheck.x && clickX < toCheck.x + toCheck.clip.w && clickY >= toCheck.y && clickY < toCheck.y + toCheck.clip.h);
}

void FightScreen::paintItem(int xLoc, int yLoc, Item* item)
{
//	applySurface(xLoc,yLoc,item->graphics->image,screen,&item->graphics->clip[item->index]);
}

void FightScreen::userExited(void) 
{
	//do something on exit
}

void FightScreen::fight() 
{

	playerInitiativeRoll = mainCharacter->getInitiativeRoll();
	std::cout << "\n Your Initiative Roll: " << playerInitiativeRoll << "\n";
	monsterInitiativeRoll = thisMonster->getInitiativeRoll();
	std::cout << "Monsters Initiative Roll: " << monsterInitiativeRoll << "\n";

	if (playerInitiativeRoll >= monsterInitiativeRoll)
	{
		std::cout << "\n YOU ATTACK FIRST! \n";

		playerAttackRoll = Dice::roll(Dice::D20) + mainCharacter->getAttackBonus();
		std::cout << "\nYour Attack Roll: " << playerAttackRoll << "\n";
		//critical hit implimentation
		if (playerAttackRoll == 20) { //critical hit implimentation
			std::cout << "YOU AUTOMATICALY HIT HIM" << "\n\n";
			damageRoll = true;
		//Normal Hit
		}
		if (playerAttackRoll >= (thisMonster->getAC())) {
			std::cout << "YOU HIT HIM!" << "\n\n";
			damageRoll = true;
		}
		//automatic miss
		if (playerAttackRoll == 1) {
			std::cout << "YOU AUTOMATICALLY MISS HIM!" << "\n\n";
			damageRoll = false;
		}
		if (damageRoll)
		{
			damageRoll = false;
			if (mainCharacter->equippedWeapon != NULL)
				playerDamageRoll = Dice::roll(mainCharacter->equippedWeapon->getDamage()) + (mainCharacter->getStrMod());
			else
				playerDamageRoll = Dice::roll(mainCharacter->HAND_COMBAT_DICE) + (mainCharacter->getStrMod());

			std::cout <<"\nYour Damage Roll: " << playerDamageRoll << "\n";

			if (playerDamageRoll <= 1) 
			{
				std::cout << "You deal an automatic 1 damage \n";
				thisMonster->setHp( (thisMonster->getHp() - 1) );
			}
			else
			{
				std::cout << "You deal " << playerDamageRoll << " damage points\n\n";
				thisMonster->setHp ( (thisMonster->getHp() - 1) );
			}
		}
	}
	else
	{
		std::cout << "\n MONSTER ATTACKS FIRST \n";

		monsterAttackRoll = Dice::roll(Dice::D20) + thisMonster->getAttackBonus();
		std::cout << "\nMonster Attack Roll: " << monsterAttackRoll << "\n";
		//critical hit implimentation
		if (monsterAttackRoll == 20) { //critical hit implimentation
			std::cout << "MONSTER AUTOMATICALY HITS YOU" << "\n\n";
			damageRoll = true;
		//Normal Hit
		}
		if (monsterAttackRoll >= (thisMonster->getAC())) {
			std::cout << "MONSTER HITS YOU!" << "\n\n";
			damageRoll = true;
		}
		//automatic miss
		if (monsterAttackRoll == 1) {
			std::cout << "MONSTER AUTOMATICALLY HITS YOU!" << "\n\n";
			damageRoll = false;
		}
		if (damageRoll)
		{
			damageRoll = false;
			monsterDamageRoll = Dice::roll(thisMonster->getDamageDiceType()) + (thisMonster->getStrMod());

			std::cout <<"\nMonster Damage Roll: " << monsterDamageRoll << "\n";

			if (monsterDamageRoll <= 1) 
			{
				std::cout << "Monster deals an automatic 1 damage \n";
				mainCharacter->setHp( (mainCharacter->getHp() - 1) );
			}
			else
			{
				std::cout << "Monster deals " << playerDamageRoll << " damage points\n\n";
				mainCharacter->setHp ( (mainCharacter->getHp() - 1) );
			}
		}
	}
}

	


