#include "FightScreen.h"
#include "SelectionScreen.h"



FightScreen::FightScreen(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{

	attackRoll = false;
	initiativeRoll = false;

	background = loadImage(".\\images\\fightScreen.png");
	buttons = loadImage(".\\images\\fightButtons.png");

	notifyCompletion = NULL;

	// initialize the buttons used to buy and sell
	rollButton.clip.x = 0;
	rollButton.clip.y = 0;
	attackButton.clip.x = 5*16;
	attackButton.clip.y = 0;
	rollButton.clip.w = attackButton.clip.w = 5*16;
	rollButton.clip.h = attackButton.clip.h = 2*16;

	rollButton.x = attackButton.x = x+(BUTTON_SECTION_X)*16;
	rollButton.y = attackButton.y = y+(BUTTON_SECTION_Y)*16;

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
	initiativeRoll = true;

	if(initiativeRoll)
	{
		applySurface(rollButton.x, rollButton.y, buttons, screen, &rollButton.clip);
	}
	else
	{
		applySurface(attackButton.x, attackButton.y, buttons, screen, &attackButton.clip);
	}

	paintStaticMessage();
	paintDynamicMessage();
}

void FightScreen::paintStaticMessage()
{

	int xi = 3.5*16;
	int yi = 1.5*16;

	//END PLAYERS NAME
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

	switch (mainCharacter->getClass())
	{
	case 0:
			msgStatic = TTF_RenderText_Solid(fontCalibriBold, "FIGHTER", fontColorWhite );
			applySurface( (x+xi), (y+yi), msgStatic, screen );
			break;
	}
	xi = xi-(4*16);

	yi = yi+(2*16);
	msgStatic = TTF_RenderText_Solid(fontCalibriBold, "HP:", fontColorRed );
	applySurface( (x+xi), (y+yi), msgStatic, screen );
	yi = yi-(2*16);

	//ENTER BADGUY NAME
	xi = xi+25*16;
	msgStatic = TTF_RenderText_Solid(fontCalibriBold, "MONSTER GUY", fontColorWhite );
	applySurface( (x+xi), (y+yi), msgStatic, screen );

	yi = yi+2*16;
	msgStatic = TTF_RenderText_Solid(fontCalibriBold, "HP:", fontColorRed );
	applySurface( (x+xi), (y+yi), msgStatic, screen );

	//PAINT BOTH STATS
	paintStats(1,1,true);
	paintStats(1,1,false);
}

void FightScreen::paintDynamicMessage()
{
	int xi = 0.5*16;
	int yi = 10.5*16;
	if (initiativeRoll) 
	{
		msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, "Your Initiative Roll:", fontColorWhite );
		applySurface( (INFO_SECTION_X+x+xi), (INFO_SECTION_Y+y+yi), msgDynamic, screen );

		msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, "Your Initiative Roll:", fontColorWhite );
		applySurface( (MONSTER_SECTION_X+x+xi), (MONSTER_SECTION_Y+y+yi), msgDynamic, screen );
	}
	if (attackRoll)
	{
		msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, "Your Attack Roll:", fontColorWhite );
		applySurface( (INFO_SECTION_X+x+xi), (INFO_SECTION_Y+y+yi), msgDynamic, screen );

		msgDynamic = TTF_RenderText_Solid(fontCalibriTiny, "Your Attack Roll:", fontColorWhite );
		applySurface( (MONSTER_SECTION_X+x+xi), (MONSTER_SECTION_Y+y+yi), msgDynamic, screen );
	}

	xi = 5.5*16;
	yi = 3.5*16;
	hp = mainCharacter->getHp();
	_itoa_s(hp,buffer,10);
	msgDynamic = TTF_RenderText_Solid(fontCalibriBold, buffer, fontColorRed );
	applySurface( (x+xi), (y+yi), msgDynamic, screen );

}


void FightScreen::paintStats(int xi, int yi, bool isPlayer)
{
	int infox, infoy;
	if (isPlayer) {
		infox = INFO_SECTION_X;
		infoy = INFO_SECTION_Y;
	}
	else {
		infox = MONSTER_SECTION_X;
		infoy = MONSTER_SECTION_Y;
	}

	msgStatic = TTF_RenderText_Solid(fontCalibri, "Level ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "AC ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Atck Bonus ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Weapon ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Weapon Dmg ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );

	yi = yi+1*16;
	msgStatic = TTF_RenderText_Solid(fontCalibri, "Initiative Roll ::", fontColorWhite );
	applySurface( (infox+x+xi), (infoy+y+yi), msgStatic, screen );
}

void FightScreen::setSignal(bool* signal)
{
	notifyCompletion = signal;
}

void FightScreen::mouseLeft(int clickX, int clickY)
{

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





