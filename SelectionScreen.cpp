#include "SelectionScreen.h"

//initialize pointers for font type screens
SDL_Surface* SelectionScreen::characterMessage = NULL;
SDL_Surface* SelectionScreen::attributes = NULL;
SDL_Surface* SelectionScreen::message = NULL;



SelectionScreen::SelectionScreen(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{
	init();
}

SelectionScreen::~SelectionScreen()
{
	SDL_FreeSurface(characterSprites);
	SDL_FreeSurface(selectScreen);
}

void SelectionScreen::init()
{
	characterSprites = loadImage(SPRITES_HERO,0,0xFF,0xFF);
	selectScreen = loadImage(SELECT_SCREEN_BACKDROP);

	//initialize to non-selected
	selectedSprite = -1;
	selectedRace = -1;
	selectedClass = -1;
	//initialize roll button to never pressed
	hasRolled = false;

	//buttons for the sprites
	availableSprites[0] = Button(5*TILE_SIZE,5*TILE_SIZE,TILE_SIZE,TILE_SIZE,0,0,characterSprites);
	availableSprites[1] = Button(9*TILE_SIZE,5*TILE_SIZE,TILE_SIZE,TILE_SIZE,9*TILE_SIZE,0,characterSprites);
	availableSprites[2] = Button(13*TILE_SIZE,5*TILE_SIZE,TILE_SIZE,TILE_SIZE,0,4*TILE_SIZE,characterSprites);
	availableSprites[3] = Button(17*TILE_SIZE,5*TILE_SIZE,TILE_SIZE,TILE_SIZE,5*TILE_SIZE,4*TILE_SIZE,characterSprites);
	availableSprites[4] = Button(21*TILE_SIZE,5*TILE_SIZE,TILE_SIZE,TILE_SIZE,9*TILE_SIZE,4*TILE_SIZE,characterSprites);

	//buttons for the races
	availableRaces[0] = Rect(3*TILE_SIZE,14*TILE_SIZE,6*TILE_SIZE,TILE_SIZE);
	availableRaces[1] = Rect(17*TILE_SIZE,18*TILE_SIZE,5*TILE_SIZE,TILE_SIZE);
	availableRaces[2] = Rect(24*TILE_SIZE,16*TILE_SIZE,3*TILE_SIZE,TILE_SIZE);
	availableRaces[3] = Rect(10*TILE_SIZE,20*TILE_SIZE,6*TILE_SIZE,TILE_SIZE);

	//buttons for the classes
	availableClasses[0] = Rect(8*TILE_SIZE,31*TILE_SIZE,6*TILE_SIZE,TILE_SIZE);
	availableClasses[1] = Rect(19*TILE_SIZE,31*TILE_SIZE,6*TILE_SIZE,TILE_SIZE);
}

void SelectionScreen::paint()
{
	// apply background image
	applySurface(0,0,selectScreen,screen);

	// paint each sprite
	for(int i=0;i<NUM_CHARACTERS;i++)
		availableSprites[i].paint();

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

	//if you have clicked the rolled button paint the stats
	if(hasRolled) {
		paintAttributes(36,8);
	}
}

//what happens when you click a roll button
//rolls for player attributes
void SelectionScreen::rollButton()
{
	rollStr = ControllableCharacter::getAbilityRoll();
	rollDex = ControllableCharacter::getAbilityRoll();
	rollCon = ControllableCharacter::getAbilityRoll();
	rollIte = ControllableCharacter::getAbilityRoll();
	rollWis = ControllableCharacter::getAbilityRoll();
	rollCha = ControllableCharacter::getAbilityRoll();

	std::cout << "\n";

	modStr = ControllableCharacter::getModifier(rollStr);
	modDex = ControllableCharacter::getModifier(rollDex);
	modCon = ControllableCharacter::getModifier(rollCon);
	modIte = ControllableCharacter::getModifier(rollIte);
	modWis = ControllableCharacter::getModifier(rollWis);
	modCha = ControllableCharacter::getModifier(rollCha);

	sStr = to_string(rollStr);
	sDex = to_string(rollDex);
	sCon = to_string(rollCon);
	sIte = to_string(rollIte);
	sWis = to_string(rollWis);
	sCha = to_string(rollCha);

	smStr = to_string(modStr);
	smDex = to_string(modDex);
	smCon = to_string(modCon);
	smIte = to_string(modIte);
	smWis = to_string(modWis);
	smCha = to_string(modCha);
}

//this paints the rolls to the screen
void SelectionScreen::paintAttributes(int x, int y)
{
	attributes = TTF_RenderText_Solid(fontCalibri, sStr.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smStr.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;

	y=y+2;
	attributes = TTF_RenderText_Solid(fontCalibri, sDex.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smDex.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;

	y=y+2;
	attributes = TTF_RenderText_Solid(fontCalibri, sCon.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smCon.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;

	y=y+2;
	attributes = TTF_RenderText_Solid(fontCalibri, sIte.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smIte.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;

	y=y+2;
	attributes = TTF_RenderText_Solid(fontCalibri, sWis.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smWis.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;

	y=y+2;
	attributes = TTF_RenderText_Solid(fontCalibri, sCha.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x+6;
	attributes = TTF_RenderText_Solid(fontCalibri, smCha.c_str(), colorWhite );
	applySurface( (x)*16, (y)*16, attributes, screen );
	x=x-6;
}

//this paints other items onto the screen that are static
void SelectionScreen::paintMessage(int x, int y)
{
	int xAdj = 5;
	message = TTF_RenderText_Solid(fontCalibri, "STR:", colorWhite );
	applySurface( (x)*16, (y)*16, message, screen );
	if (selectedRace == 3)
		message = TTF_RenderText_Solid(fontCalibri, "-", colorWhite );
	else
		message = TTF_RenderText_Solid(fontCalibri, "+", colorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 1, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", colorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "DEX:", colorWhite );
	applySurface( x*16, (y)*16, message, screen );
	if (selectedRace == 1)
		message = TTF_RenderText_Solid(fontCalibri, "-", colorWhite );
	else
		message = TTF_RenderText_Solid(fontCalibri, "+", colorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 2, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", colorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "CON:", colorWhite );
	applySurface( x*16, (y)*16, message, screen );
	if (selectedRace == 2)
		message = TTF_RenderText_Solid(fontCalibri, "-", colorWhite );
	else
		message = TTF_RenderText_Solid(fontCalibri, "+", colorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 3, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", colorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "ITE:", colorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", colorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 4, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", colorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "WIS:", colorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", colorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 5, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", colorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "CHA:", colorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", colorWhite );
	applySurface( (x+4)*16, (y)*16, message, screen );
	printADJ(selectedRace, 6, x+xAdj, y);
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", colorWhite );
	applySurface( (x+6)*16, (y)*16, message, screen );
}

//this paints the type of character you have selected dynamically
void SelectionScreen::paintCharacterMessage(int race, int clas)
{
	int x = 36*16;
	int y = 5*16+8;
	switch (race)
	{
		case -1:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, " ", colorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 0:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "HUMAN ", colorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 1:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "DWARF ", colorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 2:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "ELF ", colorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 3:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "GNOME ", colorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
	}
	x = 40*16;
	switch (clas)
	{
		case -1:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, " ", colorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 0:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "FIGHTER ", colorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
		case 1:
			characterMessage = TTF_RenderText_Solid(fontCalibriBold, "RANGER ", colorBlack );
			applySurface( x, y, characterMessage, screen );
			break;
	}
}

//simple function to output what race you selected to console
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
		default:
			std::cout << "gnome ";
			return GNOME;
	}
}

//outputs your selected class to console
playerClass SelectionScreen::findClass(int selectedClass)
{
	switch (selectedClass)
	{
		case 0:
			std::cout << "fighter.";
			return FIGHTER;
		default:
			std::cout << "ranger.";
			return RANGER;
	}
}

//this prints out the special additions to race attribtes
void SelectionScreen::printADJ(int selectedRace, int characteristic, int x, int y)
{

	x = x*16;
	y = y*16;
	switch (selectedRace)
	{
		case -1:
			message = TTF_RenderText_Solid(fontCalibri, " ", colorWhite );
			applySurface( (x+1), y, message, screen );
			break;

		case 0:
			sadjStr = to_string(abs(HumanRace::STR_ADJ));
			sadjDex = to_string(abs(HumanRace::DEX_ADJ));
			sadjCon = to_string(abs(HumanRace::CON_ADJ));
			sadjIte = to_string(abs(HumanRace::ITE_ADJ));
			sadjWis = to_string(abs(HumanRace::WIS_ADJ));
			sadjCha = to_string(abs(HumanRace::CHA_ADJ));
			switch (characteristic)
			{
				case 1:
					message = TTF_RenderText_Solid(fontCalibri, sadjStr.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 2:
					message = TTF_RenderText_Solid(fontCalibri, sadjDex.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 3:
					message = TTF_RenderText_Solid(fontCalibri, sadjCon.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 4:
					message = TTF_RenderText_Solid(fontCalibri, sadjIte.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 5:
					message = TTF_RenderText_Solid(fontCalibri, sadjWis.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 6:
					message = TTF_RenderText_Solid(fontCalibri, sadjCha.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
			}
			break;
		case 1:
			sadjStr = to_string(abs(DwarfRace::STR_ADJ));
			sadjDex = to_string(abs(DwarfRace::DEX_ADJ));
			sadjCon = to_string(abs(DwarfRace::CON_ADJ));
			sadjIte = to_string(abs(DwarfRace::ITE_ADJ));
			sadjWis = to_string(abs(DwarfRace::WIS_ADJ));
			sadjCha = to_string(abs(DwarfRace::CHA_ADJ));
			switch (characteristic)
			{
				case 1:
					message = TTF_RenderText_Solid(fontCalibri, sadjStr.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 2:
					message = TTF_RenderText_Solid(fontCalibri, sadjDex.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 3:
					message = TTF_RenderText_Solid(fontCalibri, sadjCon.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 4:
					message = TTF_RenderText_Solid(fontCalibri, sadjIte.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 5:
					message = TTF_RenderText_Solid(fontCalibri, sadjWis.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 6:
					message = TTF_RenderText_Solid(fontCalibri, sadjCha.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
			}
			break;
		case 2:
			sadjStr = to_string(abs(ElfRace::STR_ADJ));
			sadjDex = to_string(abs(ElfRace::DEX_ADJ));
			sadjCon = to_string(abs(ElfRace::CON_ADJ));
			sadjIte = to_string(abs(ElfRace::ITE_ADJ));
			sadjWis = to_string(abs(ElfRace::WIS_ADJ));
			sadjCha = to_string(abs(ElfRace::CHA_ADJ));
			switch (characteristic)
			{
				case 1:
					message = TTF_RenderText_Solid(fontCalibri, sadjStr.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 2:
					message = TTF_RenderText_Solid(fontCalibri, sadjDex.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 3:
					message = TTF_RenderText_Solid(fontCalibri, sadjCon.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 4:
					message = TTF_RenderText_Solid(fontCalibri, sadjIte.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 5:
					message = TTF_RenderText_Solid(fontCalibri, sadjWis.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 6:
					message = TTF_RenderText_Solid(fontCalibri, sadjCha.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
			}
			break;
		case 3:
			sadjStr = to_string(abs(GnomeRace::STR_ADJ));
			sadjDex = to_string(abs(GnomeRace::DEX_ADJ));
			sadjCon = to_string(abs(GnomeRace::CON_ADJ));
			sadjIte = to_string(abs(GnomeRace::ITE_ADJ));
			sadjWis = to_string(abs(GnomeRace::WIS_ADJ));
			sadjCha = to_string(abs(GnomeRace::CHA_ADJ));
			switch (characteristic)
			{
				case 1:
					message = TTF_RenderText_Solid(fontCalibri, sadjStr.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 2:
					message = TTF_RenderText_Solid(fontCalibri, sadjDex.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 3:
					message = TTF_RenderText_Solid(fontCalibri, sadjCon.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 4:
					message = TTF_RenderText_Solid(fontCalibri, sadjIte.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 5:
					message = TTF_RenderText_Solid(fontCalibri, sadjWis.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
				case 6:
					message = TTF_RenderText_Solid(fontCalibri, sadjCha.c_str(), colorWhite );
					applySurface( (x+1), y, message, screen );
					break;
			}
			break;
	}

}

//LEFT MOUSE EVENT POLLING
void SelectionScreen::mouseLeft(int x, int y)
{
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
				}
				else {
					mainCharacter = new Fighter(myRace, rollStr, rollDex, rollCon, rollIte, rollWis, rollCha);
				}
				case RANGER:
				if (!hasRolled) {
					mainCharacter = new Ranger(myRace);
				}
				else {
					mainCharacter = new Ranger(myRace, rollStr, rollDex, rollCon, rollIte, rollWis, rollCha);
				}
			}
			mainCharacter->showCharacter();
			mainCharacter->graphics = new Graphics(SPRITES_HERO);
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
		if(availableSprites[i].inBounds(x,y))
			selectedSprite = i;
	}
	//check if any of the races were selected
	for(int i=0;i<NUM_RACES;i++)
	{
		if(availableRaces[i].inBounds(x,y))
			selectedRace = i;
	}
	//check if any of the races were selected
	for(int i=0;i<NUM_CLASSES;i++)
	{
		if(availableClasses[i].inBounds(x,y))
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
		if(availableSprites[i].inBounds(x,y))
			selectedSprite = -1;
	}
	//check if any of the races were selected
	for(int i=0;i<NUM_RACES;i++)
	{
		if(availableRaces[i].inBounds(x,y))
			selectedRace = -1;
	}
	//check if any of the races were selected
	for(int i=0;i<NUM_CLASSES;i++)
	{
		if(availableClasses[i].inBounds(x,y))
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
