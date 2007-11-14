#include "Screens.h"

// #####################################################################################################

// initialize the static pointers
Character* Screen::mainCharacter = NULL;
SDL_Surface* Screen::screen = NULL;
SDL_Surface* Screen::message = NULL;
TTF_Font* Screen::font = NULL;

void Screen::setScreen(SDL_Surface *newScreen)
{
	Screen::screen = newScreen;
}

Screen::Screen(int newX, int newY, int newW, int newH)
{
	x = newX;
	y = newY;
	w = newW;
	h = newH;
}

// #####################################################################################################

SelectionScreen::SelectionScreen(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{
	init();
}

void SelectionScreen::init()
{
	if (TTF_Init() == -1)
		std::cout << "TTF NOT INITLAIZED/n";

	Screen::font = TTF_OpenFont( ".\\fonts\\calibri.ttf", 12 );

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
	
	if(hasRolled) {
				SDL_Color textColor = {255,255,255};
		message = TTF_RenderText_Solid(font, "The quick brown fox jumps over the lazy hound", textColor );
		applySurface( 0, 0, message, screen );}
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
			std::cout << "You have selected a: ";
			//decide which race you selected
			switch (selectedRace) {
				case 0:
					myRace = HUMAN;
					std::cout << "human ";
					break;
				case 1:
					myRace = DWARF;
					std::cout << "dwarf ";
					break;
				case 2:
					myRace = ELF;
					std::cout << "elf ";
					break;
				case 3:
					myRace = GNOME;
					std::cout << "gnome ";
			}
			//decide which class you selected
			switch (selectedClass) {
				case 0:
					myClass = FIGHTER;
					std::cout << "fighter.";
					break;
				case -1:
					break;
			}
			std::cout << " \n and you've chosen Character " << selectedSprite+1 << "\n\n";
			//call the character you selected
			SDL_Rect *characterRect = new SDL_Rect();

			//switch statement to call appropriate class for which class you selected
			switch (myClass) {
				case FIGHTER:
					if (!hasRolled)
						mainCharacter = new Fighter(myRace);
					else
						mainCharacter = new Fighter(myRace);
			
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
	if (x >=38*16 && x<=42*16 && y>=23*16 && y<=24*16)
	{
		hasRolled = true;

		switch (selectedRace)
		{
			case -1:
				break;
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
		}
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

// #####################################################################################################

MainGame::MainGame(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{
	currentLevel = LEVEL_1;
	gameMap = Map(16,16,672,512);
	numPlayers = -1;
	numEnemies = -1;
	currSpeed = -1;
	currentPlayer = NULL;
	shopScreen = NULL;
	weapons = NULL;
}

// begins the game by loading the level and all of its enemies
void MainGame::init()
{
	loadLevel();
	weapons = WeaponFactory::getAllWeapons();
	numPlayers = numEnemies+1;

	gameMap.loadGraphics(level->graphics, level->alphaR, level->alphaG, level->alphaB);
	gameMap.parseIndex(level->index);
	state = STATE_LEVEL_START;

	std::cout << "There are " << numEnemies << " monsters on this level\n";

	if(state == STATE_LEVEL_START)
		doInitiativeRoll();
}

// initiative roll to decide the order the turns will be taken in
// the first roll is arbitarily the human's roll
void MainGame::doInitiativeRoll()
{
	std::cout << "\nNow let's do an initiative roll to see who starts\n";
	int *rolls = new int[numPlayers];
	for(int i=0; i<numPlayers; i++)
	{
		rolls[i] = Dice::roll(Dice::D20);
		if(i==0)
			std::cout << "You rolled a " << rolls[i] << "\n";
		else
			std::cout << "Monster #" << i+1 << " rolled a " << rolls[i] << "\n";
	}
	std::cout << "\n";

	int i, j, highestRoll, highRollerIdx;

	// put the characters in the queue in the order of their initiative roll
	for(i=0;i<numPlayers;i++)
	{
		// arbitrarily choose the first roll as the highest
		highestRoll = rolls[0];
		highRollerIdx = 0;

		// see if any of the other rolls were higher
		for(j=1;j<numPlayers;j++)
		{
			if(rolls[j] > highestRoll)
			{
				highestRoll = rolls[j];
				highRollerIdx = j;
			}
		}
		// set the highest to -1, so it doesn't appear again
		rolls[highRollerIdx] = -1;

		// add the highest roller to the queue
		if(highRollerIdx == 0)
			playOrder.push(mainCharacter);
		else
			playOrder.push(&enemies[highRollerIdx-1]);
	}

	nextTurn();
}

// respect the turn order, handling AI it is an enemy's turn
void MainGame::nextTurn()
{
	if(currentPlayer != NULL)
		playOrder.push(currentPlayer);

	currentPlayer = playOrder.front();
	playOrder.pop();

	currSpeed = currentPlayer->getSpeed();

	if(currentPlayer == mainCharacter)
	{
		state = STATE_HUMAN_TURN;
		std::cout << "Your turn, make your move\n";
	}
	else 
	{
		state = STATE_AI_TURN;
	}

	// play the computer's turn
	if(state == STATE_AI_TURN)
	{
		std::cout << "AI's turn ";
		paintNow();
		SDL_Delay(500);

		for( ;currSpeed>0;currSpeed--)
		{
			std::cout << ".";
			doAITurn();
			paintNow();
			SDL_Delay(250);
		}
		std::cout << " Done!\n";
		nextTurn();
	}
}

// super simplistic AI, randomly goes up, down, left or right
void MainGame::doAITurn()
{
	switch(rand() % 4)
	{
		case 0:
			// move up
			if(currentPlayer->y > 0)
				if(gameMap.ts[currentPlayer->x][currentPlayer->y-1].isWalkable)
					currentPlayer->y -=1;
			break;
		case 1:
			// move down
			if(currentPlayer->y < gameMap.h-1)
				if(gameMap.ts[currentPlayer->x][currentPlayer->y+1].isWalkable)
					currentPlayer->y +=1;
			break;
		case 2:
			// move left
			if(currentPlayer->x > 0)
				if(gameMap.ts[currentPlayer->x-1][currentPlayer->y].isWalkable)
					currentPlayer->x -=1;
			break;
		default:
			// move right
			if(currentPlayer->x < gameMap.w-1)
				if(gameMap.ts[currentPlayer->x+1][currentPlayer->y].isWalkable)
					currentPlayer->x +=1;
			break;
	}
}

// initialize the current level, including the map and the enemies
void MainGame::loadLevel()
{
	switch(currentLevel)
	{
		case LEVEL_1:
			std::cout << "\nNow Loading Level 1\n";

			level = new Level();
			level->graphics = ".\\levels\\level00\\graphicTiles.bmp";
			level->index = ".\\levels\\level00\\index.map";
			level->alphaR = 0xFF;
			level->alphaG = 0xE2;
			level->alphaB = 0xAA;

			// initialize a small set of enemies
			Graphics *monsterGraphics = new Graphics(".\\images\\enemies.png",0xFF, 0x0, 0xFF);

			numEnemies = 3;

			enemies = new Monster[numEnemies];

			// skeleton
			enemies[0].graphics = monsterGraphics;
			enemies[0].clip->x = 6*16;
			enemies[0].clip->y = 2*16;
			enemies[0].clip->w = 16;
			enemies[0].clip->h = 16;
			enemies[0].x = 15;
			enemies[0].y = 20;
			enemies[0].setSpeed(30);

			// thug
			enemies[1].graphics = monsterGraphics;
			enemies[1].clip->x = 0;
			enemies[1].clip->y = 0;
			enemies[1].clip->w = 16;
			enemies[1].clip->h = 16;
			enemies[1].x = 25;
			enemies[1].y = 10;
			enemies[1].setSpeed(25);

			// porcupine
			enemies[2].graphics = monsterGraphics;
			enemies[2].clip->x = 14*16;
			enemies[2].clip->y = 0;
			enemies[2].clip->w = 16;
			enemies[2].clip->h = 16;
			enemies[2].x = 25;
			enemies[2].y = 25;
			enemies[2].setSpeed(20);

			break;
	}
}

void MainGame::paintNow()
{
	paint();
	SDL_Flip(screen);
}

void MainGame::paint()
{
	gameMap.paint();

	paintObject(mainCharacter);

	paintObject(&enemies[0]);
	paintObject(&enemies[1]);
	paintObject(&enemies[2]);

	if(state == STATE_SHOP)
	{
		shopScreen->paint();
	}
}

void MainGame::paintObject(Object* obj)
{
	if(gameMap.isOnScreen(obj->x,obj->y))
	{
		int x = gameMap.limit.x + (obj->x - gameMap.x)*16;
		int y = gameMap.limit.y + (obj->y - gameMap.y)*16;
		applySurface(x, y, obj->graphics->image, screen, obj->clip);
	}
}

void MainGame::keyUp()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(mainCharacter->y > 0)
		{
			if(gameMap.ts[mainCharacter->x][mainCharacter->y-1].isWalkable)
			{
				mainCharacter->y -= 1;
				currSpeed -= 1;
			}
		}
		if(currSpeed == 0)
			nextTurn();
	}
}
void MainGame::keyDown()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(mainCharacter->y < gameMap.h-1)
		{
			if(gameMap.ts[mainCharacter->x][mainCharacter->y+1].isWalkable)
			{
				mainCharacter->y += 1;
				currSpeed -= 1;
			}
		}
		if(currSpeed == 0)
			nextTurn();
	}
}
void MainGame::keyLeft()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(mainCharacter->x > 0)
		{
			if(gameMap.ts[mainCharacter->x-1][mainCharacter->y].isWalkable)
			{
				mainCharacter->x -= 1;
				currSpeed -= 1;
			}
		}
		if(currSpeed == 0)
			nextTurn();
	}
}
void MainGame::keyRight()
{
	if(state == STATE_HUMAN_TURN)
	{
		if(mainCharacter->x < gameMap.w-1)
		{
			if(gameMap.ts[mainCharacter->x+1][mainCharacter->y].isWalkable)
			{
				mainCharacter->x += 1;
				currSpeed -= 1;
			}
		}
		if(currSpeed == 0)
			nextTurn();
	}
}

void MainGame::showShop()
{
	if(state != STATE_SHOP)
	{
		std::cout << "SHOW SHOP!\n";
		if(shopScreen == NULL)
			shopScreen = new ShopScreen(50,50,400,200);
		state = STATE_SHOP;
	}
	else
	{
		std::cout << "HIDE SHOP!\n";

		// you can only enter the shop during the human turn, so restore the human turn
		state = STATE_HUMAN_TURN;
	}
}

// #####################################################################################################

ShopScreen::ShopScreen(int newX, int newY, int newW, int newH) : 
	Screen(newX,newY,newW,newH)
{
	bgColor = SDL_MapRGB( screen->format, 0xFF, 0x00, 0x00 );
	weapons = WeaponFactory::getAllWeapons();
	numWeapons = WeaponFactory::getNumWeapons();
	armor = ArmorFactory::getAllArmor();
	numArmor = ArmorFactory::getNumArmor();
}

void ShopScreen::paint()
{
	SDL_Rect tempClip;
	tempClip.x = x;
	tempClip.y = y;
	tempClip.w = w;
	tempClip.h = h;
	SDL_FillRect(screen,&tempClip,bgColor);

	for(int i=0;i<numWeapons;i++)
		applySurface(x+(i*16),y,weapons[i].graphics->image,screen,&weapons[i].graphics->clip[weapons[i].index]);

	for(int i=0;i<numArmor;i++)
		applySurface(x+(i*16),y+16,armor[i].graphics->image,screen,&armor[i].graphics->clip[armor[i].index]);
}
