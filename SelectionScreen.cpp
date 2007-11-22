#include "SelectionScreen.h"

//initialize pointers for font type screens
SDL_Surface* SelectionScreen::characterMessage = NULL;
SDL_Surface* SelectionScreen::attributes = NULL;
SDL_Surface* SelectionScreen::message = NULL;



SelectionScreen::SelectionScreen(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{
	init();
}

void SelectionScreen::init()
{
	//font colors
	textColorWhite.r = textColorWhite.g = textColorWhite.b = 0xFF;
	textColorBlack.r = textColorBlack.g = textColorBlack.b = 0x00;

	characterSprites = new Graphics(".\\images\\characters.png");
	selectScreen = loadImage(".\\images\\selectScreen.png");

	//initialize to non-selected
	selectedSprite = -1;
	selectedRace = -1;
	selectedClass = -1;
	//initialize roll button to never pressed
	hasRolled = false;

	//location of available sprites on clip sheet
	availableSprites[0].clip.x = 0;
	availableSprites[0].clip.y = 0; 
	
	availableSprites[1].clip.x = 9*16; 
	availableSprites[1].clip.y = 0; 

	availableSprites[2].clip.x = 0; 
	availableSprites[2].clip.y = 4*16;

	availableSprites[3].clip.x = 5*16; 
	availableSprites[3].clip.y = 4*16;

	availableSprites[4].clip.x = 9*16; 
	availableSprites[4].clip.y = 4*16;

	//location to paint the sprites
	for(int i=0;i<NUM_CHARACTERS;i++)
	{
		availableSprites[i].x = 80 + (i*64);
		availableSprites[i].y = 80;
		availableSprites[i].clip.w = availableSprites[i].clip.h = 16; 
	}

	//location to paint the highlight box for seleced races
	availableRaces[0].x = 3*16;
	availableRaces[0].y = 14*16;
	availableRaces[0].clip.w = 6*16;
	availableRaces[0].clip.h = 16;

	availableRaces[1].x = 17*16;
	availableRaces[1].y = 18*16;
	availableRaces[1].clip.w = 5*16;
	availableRaces[1].clip.h = 16;

	availableRaces[2].x = 24*16;
	availableRaces[2].y = 16*16;
	availableRaces[2].clip.w = 3*16;
	availableRaces[2].clip.h = 16;

	availableRaces[3].x = 10*16;
	availableRaces[3].y = 20*16;
	availableRaces[3].clip.w = 6*16;
	availableRaces[3].clip.h = 16;

	//location to paint the highlight box for selected classes
	availableClasses[0].x = 15*16;
	availableClasses[0].y = 31*16;
	availableClasses[0].clip.w = 6*16;
	availableClasses[0].clip.h = 16;
}

void SelectionScreen::paint()
{
	// apply background image
	applySurface(0,0,selectScreen,screen);

	// paint each sprite
	for(int i=0;i<NUM_CHARACTERS;i++)
		paintGraphicsSelection(availableSprites[i]);

	// highlight a selected sprite
	if(selectedSprite != -1) 
		applySurface(availableSprites[selectedSprite].x,availableSprites[selectedSprite].y,highlightTile,screen);
	if(selectedRace != -1)
		applySurface(availableRaces[selectedRace].x,availableRaces[selectedRace].y,highlightTile,screen);
	if(selectedClass != -1)
		applySurface(availableClasses[selectedClass].x,availableClasses[selectedClass].y,highlightTile,screen);

	//paints static message
	paintMessage(33, 8);


	//paints dynamic selected race and class
	paintCharacterMessage(selectedRace, selectedClass);
	
	if(hasRolled) {
		paintAttributes(36,8);
	}
}

//paints the sprites
void SelectionScreen::paintGraphicsSelection(GraphicsSelection &ss)
{
	applySurface(ss.x,ss.y,characterSprites->image,screen,&ss.clip);
}

void SelectionScreen::rollButton() 
{
	rollStr = ControllableCharacter::getAbilityRoll();
	rollDex = ControllableCharacter::getAbilityRoll();
	rollCon = ControllableCharacter::getAbilityRoll();
	rollIte = ControllableCharacter::getAbilityRoll();
	rollWis = ControllableCharacter::getAbilityRoll();
	rollCha = ControllableCharacter::getAbilityRoll();

	modStr = ControllableCharacter::getModifier(rollStr);
	modDex = ControllableCharacter::getModifier(rollDex);
	modCon = ControllableCharacter::getModifier(rollCon);
	modIte = ControllableCharacter::getModifier(rollIte);
	modWis = ControllableCharacter::getModifier(rollWis);
	modCha = ControllableCharacter::getModifier(rollCha);

	_itoa_s(rollStr,sStr,10); 
	_itoa_s(rollDex,sDex,10); 
	_itoa_s(rollCon,sCon,10); 
	_itoa_s(rollIte,sIte,10); 
	_itoa_s(rollWis,sWis,10); 
	_itoa_s(rollCha,sCha,10); 

	_itoa_s(modStr,smStr,10);
	_itoa_s(modDex,smDex,10); 
	_itoa_s(modCon,smCon,10); 
	_itoa_s(modIte,smIte,10); 
	_itoa_s(modWis,smWis,10); 
	_itoa_s(modCha,smCha,10); 
}

void SelectionScreen::paintAttributes(int x, int y)
{
	attributes = TTF_RenderText_Solid(fontCalibri, sStr, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smStr, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;

	y=y+2;
	attributes = TTF_RenderText_Solid(fontCalibri, sDex, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smDex, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;

	y=y+2;
	attributes = TTF_RenderText_Solid(fontCalibri, sCon, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smCon, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;

	y=y+2;
	attributes = TTF_RenderText_Solid(fontCalibri, sIte, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smIte, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;

	y=y+2;
	attributes = TTF_RenderText_Solid(fontCalibri, sWis, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smWis, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;

	y=y+2;
	attributes = TTF_RenderText_Solid(fontCalibri, sCha, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smCha, textColorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;
}

void SelectionScreen::paintMessage(int x, int y)
{
	int xAdj = 5;
	message = TTF_RenderText_Solid(fontCalibri, "STR:", textColorWhite );
	applySurface( (x)*16, (y)*16, message, screen );
	if (selectedRace == 3)
		message = TTF_RenderText_Solid(fontCalibri, "-", textColorWhite );
	else
		message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 1, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "DEX:", textColorWhite );
	applySurface( x*16, (y)*16, message, screen );
	if (selectedRace == 1)
		message = TTF_RenderText_Solid(fontCalibri, "-", textColorWhite );
	else
		message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 2, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "CON:", textColorWhite );
	applySurface( x*16, (y)*16, message, screen );
	if (selectedRace == 2)
		message = TTF_RenderText_Solid(fontCalibri, "-", textColorWhite );
	else
		message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 3, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "ITE:", textColorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 4, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "WIS:", textColorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 5, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "CHA:", textColorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 6, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );
}

void SelectionScreen::paintCharacterMessage(int race, int clas) 
{
	int x = 36*16;
	int y = 5*16+8;
	switch (race)
	{
		case -1:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, " ", textColorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 0:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "HUMAN ", textColorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 1:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "DWARF ", textColorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 2:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "ELF ", textColorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 3:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "GNOME ", textColorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
	}
	x = 40*16;
	switch (clas)
	{
		case -1:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, " ", textColorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 0:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "FIGHTER ", textColorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
	}
}

string SelectionScreen::int2string(const int i)
{
	std::ostringstream stream;
	stream << i;
	return stream.str();
}

race SelectionScreen::findRace(int selectedRace)
{
	std::cout << "You have selected a: ";
	//decide which race you selected
	switch (selectedRace) {
		case 0:
			std::cout << "human ";
			return HUMAN;
		case 1:
			std::cout << "dwarf ";
			return DWARF;
		case 2:
			std::cout << "elf ";
			return ELF;
		case 3:
			std::cout << "gnome ";
			return GNOME;
	}
}

playerClass SelectionScreen::findClass(int selectedClass)
{
	switch (selectedClass) 
	{
		case 0:
			std::cout << "fighter.";
			return FIGHTER;
	}
}

void SelectionScreen::printADJ(int selectedRace, int characteristic, int x, int y)
{

	x = x*16;
	y = y*16;
	switch (selectedRace) 
	{
		case -1:
			message = TTF_RenderText_Solid(fontCalibri, " ", textColorWhite );
			applySurface( (x+1), y, message, screen );
			break;

		case 0:
			_itoa_s(abs(HumanRace::STR_ADJ),sadjStr,10);
			_itoa_s(abs(HumanRace::DEX_ADJ),sadjDex,10);
			_itoa_s(abs(HumanRace::CON_ADJ),sadjCon,10);
			_itoa_s(abs(HumanRace::ITE_ADJ),sadjIte,10);
			_itoa_s(abs(HumanRace::WIS_ADJ),sadjWis,10);
			_itoa_s(abs(HumanRace::CHA_ADJ),sadjCha,10);
			switch (characteristic)
			{
				case 1:
					message = TTF_RenderText_Solid(fontCalibri, sadjStr, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 2:
					message = TTF_RenderText_Solid(fontCalibri, sadjDex, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 3:
					message = TTF_RenderText_Solid(fontCalibri, sadjCon, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 4:
					message = TTF_RenderText_Solid(fontCalibri, sadjIte, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 5:
					message = TTF_RenderText_Solid(fontCalibri, sadjWis, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 6:
					message = TTF_RenderText_Solid(fontCalibri, sadjCha, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
			}
			break;
		case 1:
			_itoa_s(abs(DwarfRace::STR_ADJ),sadjStr,10);
			_itoa_s(abs(DwarfRace::DEX_ADJ),sadjDex,10);
			_itoa_s(abs(DwarfRace::CON_ADJ),sadjCon,10);
			_itoa_s(abs(DwarfRace::ITE_ADJ),sadjIte,10);
			_itoa_s(abs(DwarfRace::WIS_ADJ),sadjWis,10);
			_itoa_s(abs(DwarfRace::CHA_ADJ),sadjCha,10);
			switch (characteristic)
			{
				case 1:
					message = TTF_RenderText_Solid(fontCalibri, sadjStr, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 2:
					message = TTF_RenderText_Solid(fontCalibri, sadjDex, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 3:
					message = TTF_RenderText_Solid(fontCalibri, sadjCon, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 4:
					message = TTF_RenderText_Solid(fontCalibri, sadjIte, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 5:
					message = TTF_RenderText_Solid(fontCalibri, sadjWis, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 6:
					message = TTF_RenderText_Solid(fontCalibri, sadjCha, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
			}
			break;
		case 2:
			_itoa_s(abs(ElfRace::STR_ADJ),sadjStr,10);
			_itoa_s(abs(ElfRace::DEX_ADJ),sadjDex,10);
			_itoa_s(abs(ElfRace::CON_ADJ),sadjCon,10);
			_itoa_s(abs(ElfRace::ITE_ADJ),sadjIte,10);
			_itoa_s(abs(ElfRace::WIS_ADJ),sadjWis,10);
			_itoa_s(abs(ElfRace::CHA_ADJ),sadjCha,10);
			switch (characteristic)
			{
				case 1:
					message = TTF_RenderText_Solid(fontCalibri, sadjStr, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 2:
					message = TTF_RenderText_Solid(fontCalibri, sadjDex, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 3:
					message = TTF_RenderText_Solid(fontCalibri, sadjCon, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 4:
					message = TTF_RenderText_Solid(fontCalibri, sadjIte, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 5:
					message = TTF_RenderText_Solid(fontCalibri, sadjWis, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 6:
					message = TTF_RenderText_Solid(fontCalibri, sadjCha, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
			}
			break;
		case 3:
			_itoa_s(abs(GnomeRace::STR_ADJ),sadjStr,10);
			_itoa_s(abs(GnomeRace::DEX_ADJ),sadjDex,10);
			_itoa_s(abs(GnomeRace::CON_ADJ),sadjCon,10);
			_itoa_s(abs(GnomeRace::ITE_ADJ),sadjIte,10);
			_itoa_s(abs(GnomeRace::WIS_ADJ),sadjWis,10);
			_itoa_s(abs(GnomeRace::CHA_ADJ),sadjCha,10);
			switch (characteristic)
			{
				case 1:
					message = TTF_RenderText_Solid(fontCalibri, sadjStr, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 2:
					message = TTF_RenderText_Solid(fontCalibri, sadjDex, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 3:
					message = TTF_RenderText_Solid(fontCalibri, sadjCon, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 4:
					message = TTF_RenderText_Solid(fontCalibri, sadjIte, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 5:
					message = TTF_RenderText_Solid(fontCalibri, sadjWis, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 6:
					message = TTF_RenderText_Solid(fontCalibri, sadjCha, textColorWhite );
					applySurface( (x+1), y, message, screen );
					break;
			}
			break;
	}

}

bool SelectionScreen::inBounds(GraphicsSelection &gs, int x, int y)
{
	return (x >= gs.x && x <= (gs.x + gs.clip.w) && y >= gs.y && y <= (gs.y + gs.clip.h));
}

//LEFT MOUSE EVENT POLLING
void SelectionScreen::mouseLeft(int x, int y)
{
	//DEBUG QUICKSTART!! CLICK TOP CORNER
	if (x >= 0 && x <= 2*16 && y >= 0 && y <= 2*16)
	{
			selectedSprite = 1;
			selectedRace = 0;
			selectedClass = 0;
			myRace = HUMAN; //WHEN DISABLED INFINITE WALKING WOOT!
			SDL_Rect *characterRect = new SDL_Rect();
			mainCharacter = new Fighter(myRace, rollStr, rollDex, rollCon, rollIte, rollWis, rollCha);
			mainCharacter->graphics = characterSprites;
			characterRect->x = availableSprites[selectedSprite].clip.x;
			characterRect->y = availableSprites[selectedSprite].clip.y;
			characterRect->w = availableSprites[selectedSprite].clip.w;
			characterRect->h = availableSprites[selectedSprite].clip.h;
			mainCharacter->x = 10;
			mainCharacter->y = 14;
			mainCharacter->clip = characterRect;
			signalCompletion();
	}


	//START BUTTON
	if (x >= 672 && x <= 784 && y >= 544 && y <= 591)
	{
		//Check to make sure you have selected everything
		if (selectedSprite != -1 && selectedRace != -1 && selectedClass != -1) 
		{


			myRace = findRace(selectedRace);
			myClass = findClass(selectedClass);
			
			std::cout << " \n and you've chosen Character " << selectedSprite+1 << "\n\n";
			//call the character you selected
			SDL_Rect *characterRect = new SDL_Rect();

			//switch statement to call appropriate class for which class you selected
			
			switch (myClass) 
			{
				case FIGHTER:
				if (!hasRolled) {
					mainCharacter = new Fighter(myRace);
					mainCharacter->showCharacter();
				}
				else
					mainCharacter = new Fighter(myRace, rollStr, rollDex, rollCon, rollIte, rollWis, rollCha);
					mainCharacter->showCharacter();
			}

			mainCharacter->graphics = characterSprites;
			characterRect->x = availableSprites[selectedSprite].clip.x;
			characterRect->y = availableSprites[selectedSprite].clip.y;
			characterRect->w = availableSprites[selectedSprite].clip.w;
			characterRect->h = availableSprites[selectedSprite].clip.h;

			mainCharacter->clip = characterRect;

			//pass a call back to Game.cpp
			signalCompletion();
		}
		else
			std::cout << "ERROR: You must select a character, race and class!\n";
	}
	//END START BUTTON

	//CLEAR BUTTON
	if (x >= 672 && x <= 784 && y >= 480 && y <= 527)
	{
		selectedSprite = -1;
		selectedRace = -1;
		selectedClass = -1;
	}
	//END CLEAR BUTTON

	//ROLL BUTTON
	if (x >=37*16 && x<=42*16 && y>=22*16 && y<=24*16)
	{
		hasRolled = true;
		rollButton();
	}
	//END ROLL BUTTON

	// check if any of the sprites were selected
	for(int i=0;i<NUM_CHARACTERS;i++)
	{
		if(inBounds(availableSprites[i],x,y))
			selectedSprite = i;
	}
	//check if any of the races were selected
	for(int i=0;i<NUM_RACES;i++)
	{
		if(inBounds(availableRaces[i],x,y))
			selectedRace = i;
	}
	//check if any of the races were selected
	for(int i=0;i<NUM_CLASSES;i++)
	{
		if(inBounds(availableClasses[i],x,y))
			selectedClass = i;
	}
}
//END LEFT MOUSE EVENT POLLING

//RIGHT MOUSE EVENT POLLING
void SelectionScreen::mouseRight(int x, int y)
{
	// check if any of the sprites were selected
	for(int i=0;i<NUM_CHARACTERS;i++)
	{
		if(inBounds(availableSprites[i],x,y))
			selectedSprite = -1;
	}
	//check if any of the races were selected
	for(int i=0;i<NUM_RACES;i++)
	{
		if(inBounds(availableRaces[i],x,y))
			selectedRace = -1;
	}
	//check if any of the races were selected
	for(int i=0;i<NUM_CLASSES;i++)
	{
		if(inBounds(availableClasses[i],x,y))
			selectedClass = -1;
	}
}
//END RIGHT MOUSE POLLING

//absolute value function
int SelectionScreen::abs(int x)
{
	if (x < 0)
		return (x = (x*x) + (x));
	else
		return x;
}
