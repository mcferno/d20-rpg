#include "FightScreen.h"




FightScreen::FightScreen(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{

	//temp for now
	
	Graphics* monsterGraphics = new Graphics(".\\images\\enemies.png",0xFF, 0x0, 0xFF);
	thisMonster = new Monster(15,20,6,2,monsterGraphics,Monster::SKELETON,2);

	playerHp = mainCharacter->getHp();
	monsterHp = thisMonster->getHp();
	
	attackRoll = false;
	initiativeRoll = true;
	attacked = false;
	hasRolledInitiative = false;

	lastPlayerAttack = 0;
	lastMonsterAttack = 0;
	playerInitiative = 0;
	monsterInitiative = 0;

	background = loadImage(".\\images\\fightScreen.png");
	buttons = loadImage(".\\images\\fightButtons.png");

	notifyCompletion = NULL;

	// initialize the buttons used to buy and sell
	rollButton.clip.x = 0;
	rollButton.clip.y = attackButton.clip.y = inventoryButton.clip.y = runButton.clip.y = 0;
	attackButton.clip.x = 5*16;
	inventoryButton.clip.x=10*16;
	runButton.clip.x=15*16;
	rollButton.clip.w = attackButton.clip.w = inventoryButton.clip.w = runButton.clip.w = 5*16;
	rollButton.clip.h = attackButton.clip.h = inventoryButton.clip.h = runButton.clip.h = 2*16;


	rollButton.x = attackButton.x = x+(BUTTON_SECTION_X);
	rollButton.y = attackButton.y = y+(BUTTON_SECTION_Y);

	inventoryButton.x = x+(5*16)+(BUTTON_SECTION_X);
	inventoryButton.y = y+(BUTTON_SECTION_Y);

	runButton.x = x+(10*16)+(BUTTON_SECTION_X);
	runButton.y = y+(BUTTON_SECTION_Y);

	fontColorWhite.r = fontColorWhite.g = fontColorWhite.b = 0xFF;
	fontColorRed.r = 0xFF;
	fontColorRed.g = fontColorRed.b = 0x00;


	msgInitiativeRoll = NULL;
	msgAttackRoll = NULL;
	msgStatic = NULL;
	msgDynamic = NULL;
}

void FightScreen::paint()
{
	applySurface(x,y,background,screen);
	//paint buttons
	if(initiativeRoll)
	{
		msgDynamic = TTF_RenderText_Solid(fontCalibriBold, "Roll Initiative!", fontColorRed );
		applySurface( (x+BUTTON_SECTION_X+4)+3*16, (y+BUTTON_SECTION_Y)-2*16, msgDynamic, screen );
		applySurface(rollButton.x, rollButton.y, buttons, screen, &rollButton.clip);
	}
	else
	{
		if (attacked) {
			msgDynamic = TTF_RenderText_Solid(fontCalibriBold, "Your last hit inflicted:      Damage", fontColorRed );
			applySurface( (x+BUTTON_SECTION_X+4), (y+BUTTON_SECTION_Y)-3*16, msgDynamic, screen );
			msgDynamic = TTF_RenderText_Solid(fontCalibriBold, "His last hit inflicted:         Damage", fontColorRed );
			applySurface( (x+BUTTON_SECTION_X+4), (y+BUTTON_SECTION_Y)-2*16, msgDynamic, screen );

			_itoa_s(lastPlayerAttack,buffer,10);
			msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
			applySurface( (x+BUTTON_SECTION_X+4)+9.5*16, (y+BUTTON_SECTION_Y)-3*16, msgDynamic, screen );
			_itoa_s(lastMonsterAttack,buffer,10);
			msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
			applySurface( (x+BUTTON_SECTION_X+4)+9.5*16, (y+BUTTON_SECTION_Y)-2*16, msgDynamic, screen );
		}
		applySurface(attackButton.x, attackButton.y, buttons, screen, &attackButton.clip);
	}
	applySurface(inventoryButton.x, inventoryButton.y, buttons, screen, &inventoryButton.clip);
	applySurface(runButton.x, runButton.y, buttons, screen, &runButton.clip);	


	paintStaticMessage();
	paintDynamicMessage();
}

void FightScreen::paintStaticMessage()
{

	int xi = 3.5*16;
	int yi = 1.5*16;

	//ENTER PLAYERS NAME
	switch (mainCharacter->getRace())
	{
		case 0:
			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "HUMAN", fontColorWhite );
			applySurface( (x+xi), ((y+yi)), msgStatic, screen );
			break;
		case 1:
			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "DWARF", fontColorWhite );
			applySurface( (x+xi), ((y+yi)), msgStatic, screen );
			break;
		case 2:
			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "ELF", fontColorWhite );
			applySurface( (x+xi), ((y+yi)), msgStatic, screen );
			break;
		case 3:
			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "GNOME", fontColorWhite );
			applySurface( (x+xi), (y+yi), msgStatic, screen );
			break;
	}
	xi = xi+(4*16);

	switch ((mainCharacter->getClass()))
	{
	case 0:
			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "FIGHTER", fontColorWhite );
			applySurface( (x+xi), (y+yi), msgStatic, screen );
			break;
	case 1:
			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "RANGER", fontColorWhite );
			applySurface( (x+xi), (y+yi), msgStatic, screen );
			break;
	}
	xi = xi-(4*16);

	//ENTER PLAYER HP
	yi = yi+(2*16);
	msgStatic = TTF_RenderText_Solid(fontCalibriBold, "HP:", fontColorRed );
	applySurface( (x+xi), (y+yi), msgStatic, screen );
	
	yi = yi-(2*16);

	//ENTER BADGUY NAME
	xi = xi+25*16;
	char* temp = thisMonster->getName();
	msgStatic = TTF_RenderText_Solid(fontCalibriBold, temp , fontColorWhite );
	applySurface( (x+xi), (y+yi), msgStatic, screen );

	//ENTER MONSTER HP
	yi = yi+2*16;
	msgStatic = TTF_RenderText_Solid(fontCalibriBold, "HP:", fontColorRed );
	applySurface( (x+xi), (y+yi), msgStatic, screen );

	//PAINT BOTH STATS
	paintStats();


}

void FightScreen::paintDynamicMessage()
{
	int xi = 0.5*16;
	int yi = 10.5*16;
	if (initiativeRoll) 
	{
		msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, "Your Initiative Roll:", fontColorWhite );
		applySurface( (INFO_SECTION_X+x+xi), (INFO_SECTION_Y+y+yi), msgDynamic, screen );

		msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, "His Initiative Roll:", fontColorWhite );
		applySurface( (MONSTER_SECTION_X+x+xi), (MONSTER_SECTION_Y+y+yi), msgDynamic, screen );
	}
	if (attackRoll)
	{
		msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, "Your Attack Roll:", fontColorWhite );
		applySurface( (INFO_SECTION_X+x+xi), (INFO_SECTION_Y+y+yi), msgDynamic, screen );

		msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, "His Attack Roll:", fontColorWhite );
		applySurface( (MONSTER_SECTION_X+x+xi), (MONSTER_SECTION_Y+y+yi), msgDynamic, screen );
	}

	xi = 5.5*16;
	yi = 3.5*16;
	
	//DYNAMIC PLAYER HP
	_itoa_s(playerHp,buffer,10);
	msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
	applySurface( (x+xi), (y+yi), msgDynamic, screen );
	//DYNAMIC MONSTER HP
	_itoa_s(monsterHp,buffer,10);
	msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
	applySurface( (x+xi)+25*16, (y+yi), msgDynamic, screen );

	//DYNAMIC INITIATIVE PLAYER ROLL
	_itoa_s(playerInitiative,buffer,10);
	msgDynamic = TTF_RenderText_Solid(fontCalibri, buffer, fontColorWhite );
	applySurface( (INFO_SECTION_X+x+8*16), (INFO_SECTION_Y+y+5*16), msgDynamic, screen );

	//DYNAMIC INITIATIVE MONSTER ROLL
	_itoa_s(monsterInitiative,buffer,10);
	msgDynamic = TTF_RenderText_Solid(fontCalibri, buffer, fontColorWhite );
	applySurface( (MONSTER_SECTION_X+x+8*16), (MONSTER_SECTION_Y+y+5*16), msgDynamic, screen );

}


void FightScreen::paintStats()
{
	int infox, infoy;
	int xi, yi;

	if (!hasRolledInitiative)
	{
	std::cout << "\nPLAYER STATS GENERATION: \n";
	level = mainCharacter->getLevel();
	ac = mainCharacter->getAC();
	ackbonus = mainCharacter->getAttackBonus();
	if (mainCharacter->equippedWeapon != NULL)
		weaponName = mainCharacter->equippedWeapon->getName();
	else
		weaponName = "Hands";
	if (mainCharacter->equippedWeapon != NULL)
		weapondmg = mainCharacter->equippedWeapon->getDamage();
	else
		weapondmg = 3;

	std::cout << "\nMONSTER STATS GENERATION: \n";
	mlevel = thisMonster->getLevel();
	mac = thisMonster->getAC();
	mackbonus = thisMonster->getAttackBonus();
	mweaponName = "Hands";
	mweapondmg = thisMonster->getDamageDiceType();
	}

	infox = INFO_SECTION_X;
	infoy = INFO_SECTION_Y;
	xi = 1;
	yi = 1;

	//DRAW HUMAN STATS
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Level ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
	_itoa_s(level,buffer,10);
	msgStatic = TTF_RenderText_Solid(fontCalibri, buffer, fontColorWhite );
	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );
	
	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "AC ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
	_itoa_s(ac,buffer,10);
	msgStatic = TTF_RenderText_Solid(fontCalibri, buffer, fontColorWhite );
	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Atck Bonus ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
	_itoa_s(ackbonus,buffer,10);
	msgStatic = TTF_RenderText_Solid(fontCalibri, buffer, fontColorWhite );
	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Weapon ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
	msgStatic = TTF_RenderText_Solid(fontCalibri, weaponName, fontColorWhite );
	applySurface( (infox+x+xi+4.5*16), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Weapon Dmg ::   1d", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
	_itoa_s(weapondmg,buffer,10);
	msgStatic = TTF_RenderText_Solid(fontCalibri, buffer, fontColorWhite );
	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Initiative Roll ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );

	//DRAW MONSTER STATS
	infox = MONSTER_SECTION_X;
	infoy = MONSTER_SECTION_Y;
	yi = 1;
	xi = 1;

	msgStatic = TTF_RenderText_Solid(fontCalibri, "Level ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
	_itoa_s(mlevel,buffer,10);
	msgStatic = TTF_RenderText_Solid(fontCalibri, buffer, fontColorWhite );
	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );
	
	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "AC ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
	_itoa_s(mac,buffer,10);
	msgStatic = TTF_RenderText_Solid(fontCalibri, buffer, fontColorWhite );
	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Atck Bonus ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
	_itoa_s(mackbonus,buffer,10);
	msgStatic = TTF_RenderText_Solid(fontCalibri, buffer, fontColorWhite );
	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Weapon ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
	msgStatic = TTF_RenderText_Solid(fontCalibri, mweaponName, fontColorWhite );
	applySurface( (infox+x+xi+4.5*16), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Weapon Dmg ::   1d", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
	_itoa_s(mweapondmg,buffer,10);
	msgStatic = TTF_RenderText_Solid(fontCalibri, buffer, fontColorWhite );
	applySurface( (infox+x+xi+8*16), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Initiative Roll ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );

}

void FightScreen::mouseLeft(int clickX, int clickY)
{
	if (!hasRolledInitiative)
	{
		if (clickedButton(clickX, clickY, rollButton))
		{
			playerInitiative = mainCharacter->getInitiativeRoll();
			monsterInitiative = thisMonster->getInitiativeRoll();
			hasRolledInitiative = true;
			initiativeRoll = false;
			attackRoll = true;
		}
	}

	if (hasRolledInitiative) 
	{
		if (clickedButton(clickX, clickY, attackButton))
		{
			//ALGORITHMS AND STUFF FROM ATTACK :(
		}
	}

	if (clickedButton(x+5+BUTTON_SECTION_X, y+BUTTON_SECTION_Y, inventoryButton))
	{
		//SHOW INVENTORY 
	}

	if (clickedButton(x+10+BUTTON_SECTION_X, y+BUTTON_SECTION_Y, runButton))
	{
		//EXIT O DEAR
	}
}


void FightScreen::mouseRight(int clickX, int clickY)
{
	//GOGGLES DO NOTHING
}

bool FightScreen::clickedButton(int clickX, int clickY, Button toCheck)
{
	// check the bounds of the button
	return (clickX >= toCheck.x && clickX < toCheck.x + toCheck.clip.w && clickY >= toCheck.y && clickY < toCheck.y + toCheck.clip.h);
}

void FightScreen::paintItem(int xLoc, int yLoc, Item* item)
{
	applySurface(xLoc,yLoc,item->graphics->image,screen,&item->graphics->clip[item->index]);
}

void FightScreen::userExited(void) 
{
	//do something on exit
}





