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
	if (TTF_Init() == -1)
		std::cout << "TTF NOT INITLAIZED/n";

	//font colors
	SDL_Color textColorWhite = {255,255,255};
	SDL_Color textColorBlack = {0xFF, 0xFF, 0xFF};
	//font types
	Screen::fontCalibri = TTF_OpenFont( ".\\fonts\\calibri.ttf", 16 );
	Screen::fontCalibriBold = TTF_OpenFont ( ".\\fonts\\calibrib.ttf", 16 );

	characterSprites = new Graphics(".\\images\\characters.png");
	selectScreen = loadImage(".\\images\\selectScreen.png");
	highlightTile = loadImage(".\\images\\highlight.png",0x0,0xFF,0xFF);

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

void SelectionScreen::setSignal(bool* signal)
{
	signalCompletion = signal;
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

	paintMessage(34, 8);
	paintCharacterMessage(selectedRace, selectedClass);

	
	if(hasRolled) {
		
		int temp = 4;	
		string s1;
		s1 = "not";
		

		//sprintf(s1, "%d",temp);
		std::cout << s1;
		//message = TTF_RenderText_Solid(fontCalibri, "The quick brown fox BANANAS jumps over the lazy hound", textColorWhite );
		//applySurface( 30, 0, message, screen );
		//message = TTF_RenderText_Solid(font, s1, textColor );
		//applySurface( 30, 100, message, screen );
	}
}

//paints the sprites
void SelectionScreen::paintGraphicsSelection(GraphicsSelection &ss)
{
	applySurface(ss.x,ss.y,characterSprites->image,screen,&ss.clip);
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

			mainCharacter->x = 10;
			mainCharacter->y = 14;

			mainCharacter->clip = characterRect;

			//pass a call back to Game.cpp
			*signalCompletion = true;
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

void SelectionScreen::rollButton() 
{
	rollStr = Character::getAbilityRoll();
	rollDex = Character::getAbilityRoll();
	rollCon = Character::getAbilityRoll();
	rollIte = Character::getAbilityRoll();
	rollWis = Character::getAbilityRoll();
	rollCha = Character::getAbilityRoll();
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

bool SelectionScreen::inBounds(GraphicsSelection &gs, int x, int y)
{
	return (x >= gs.x && x <= (gs.x + gs.clip.w) && y >= gs.y && y <= (gs.y + gs.clip.h));
}

//Right mouse even polling
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

void SelectionScreen::paintMessage(int x, int y)
{
	message = TTF_RenderText_Solid(fontCalibri, "STR:", textColorWhite );
	applySurface( (x)*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+3)*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+5)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "DEX:", textColorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+3)*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+5)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "CON:", textColorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+3)*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+5)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "ITE:", textColorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+3)*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+5)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "WIS:", textColorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+3)*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+5)*16, (y)*16, message, screen );

	y = y+2;
	message = TTF_RenderText_Solid(fontCalibri, "CHA:", textColorWhite );
	applySurface( x*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "+", textColorWhite );
	applySurface( (x+3)*16, (y)*16, message, screen );
	message = TTF_RenderText_Solid(fontCalibri, "MOD:", textColorWhite );
	applySurface( (x+5)*16, (y)*16, message, screen );
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