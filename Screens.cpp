#include "Screens.h"

#include <iostream>

Graphics *characterSprites = NULL;
Graphics *highlightTile = NULL;
Graphics *selectScreen = NULL;
Character *mainCharacter = NULL;

// temporary
Monster monsters[3];
Character *players[4];
int currentPlayer = -1;
int currSpeed = -1;


void SelectionScreen::init()
{
	characterSprites = new Graphics(".\\images\\characters.bmp");
	selectScreen = new Graphics(".\\images\\selectScreen.png");
	highlightTile = new Graphics(".\\images\\highlight.png");

	selectedSprite = -1;

	availableSprites[0].clip.x = 0;
	availableSprites[0].clip.y = 0; 
	
	availableSprites[1].clip.x = 272; 
	availableSprites[1].clip.y = 0; 

	availableSprites[2].clip.x = 0; 
	availableSprites[2].clip.y = 152;

	availableSprites[3].clip.x = 152; 
	availableSprites[3].clip.y = 152;

	availableSprites[4].clip.x = 272; 
	availableSprites[4].clip.y = 152;

	for(int i=0;i<NUM_CHARACTERS;i++)
	{
		availableSprites[i].x = 80 + (i*64);
		availableSprites[i].y = 96;
		availableSprites[i].clip.w = availableSprites[i].clip.h = 16; 
	}
}

SelectionScreen::SelectionScreen()
{
}

SelectionScreen::SelectionScreen(SDL_Surface *newScreen)
{
	screen = newScreen;
	init();
}

void SelectionScreen::paint()
{
	// paint background
	// if player graphic selected? paint in selected character area
	// if race selected? 

	// apply background image
	applySurface(0,0,selectScreen->image,screen);

	// paint each sprite
	for(int i=0;i<NUM_CHARACTERS;i++)
		paintSpriteSelection(availableSprites[i]);

	// highlight a selected sprite
	if(selectedSprite != -1)
		applySurface(availableSprites[selectedSprite].x,availableSprites[selectedSprite].y,highlightTile->image,screen);
}

void SelectionScreen::paintSpriteSelection(SpriteSelection ss)
{
	applySurface(ss.x,ss.y,characterSprites->image,screen,&ss.clip);
}

void SelectionScreen::mouseLeft(int x, int y)
{
	//what happens when you select START
	if (x >= 672 && x <= 784 && y >= 544 && y <= 591)
	{
		//state = STATE_MAIN_GAME;
		//mainGame.init();
	}

	// check if any of the sprites were selected
	for(int i=0;i<NUM_CHARACTERS;i++)
	{
		if (x>=availableSprites[i].x && x<=(availableSprites[i].x+16) && y>=availableSprites[i].y && y<=(availableSprites[i].y+16))
			selectedSprite = i;
	}
}

void SelectionScreen::mouseRight(int x, int y)
{
	selectedSprite = -1;

}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


MainGame::MainGame()
{
}

MainGame::MainGame(SDL_Surface *newScreen)
{
	screen = newScreen;
	currentLevel = LEVEL_1;
	gameMap = Map(16,16,672,512);
}

void MainGame::init()
{
	loadLevel();
	gameMap.loadGraphics(level->graphics, level->alphaR, level->alphaG, level->alphaB);
	gameMap.parseIndex(level->index);
	state = STATE_LEVEL_START;

	// hacked in, fake character
	mainCharacter = new Fighter(HUMAN);
	mainCharacter->graphics = characterSprites;
	SDL_Rect *tempRect = new SDL_Rect();

	tempRect->x = 0;
	tempRect->y = 0;
	tempRect->w = 16;
	tempRect->h = 16;

	mainCharacter->x = 10;
	mainCharacter->y = 14;

	mainCharacter->clip = tempRect;

	if(state == STATE_LEVEL_START)
		doInitiativeRoll();
}

// no actual rolling of dice to decide order just yet
void MainGame::doInitiativeRoll()
{
	players[0] = mainCharacter;
	players[1] = &monsters[0];
	players[2] = &monsters[1];
	players[3] = &monsters[2];

	nextTurn();
}

void MainGame::nextTurn()
{
	std::cout << "current turn : " << currentPlayer << "\n";
	currentPlayer +=1;
	currentPlayer %= 4;
	currSpeed = 5;

	if(players[currentPlayer] == mainCharacter)
	{
		state = STATE_HUMAN_TURN;
		std::cout << "human turn!\n";
	}
	else 
	{
		state = STATE_AI_TURN;
		std::cout << "AI turn!\n";
	}

	if(state == STATE_AI_TURN)
	{
		for(;currSpeed>=0;currSpeed--)
		{
			doAITurn();
			paintNow();
			SDL_Delay(250);
		}
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
			if(players[currentPlayer]->y > 0)
				if(gameMap.ts[players[currentPlayer]->x][players[currentPlayer]->y-1].isWalkable)
					players[currentPlayer]->y -=1;
			break;
		case 1:
			// move down
			if(players[currentPlayer]->y < gameMap.h-1)
				if(gameMap.ts[players[currentPlayer]->x][players[currentPlayer]->y+1].isWalkable)
					players[currentPlayer]->y +=1;
			break;
		case 2:
			// move left
			if(players[currentPlayer]->x > 0)
				if(gameMap.ts[players[currentPlayer]->x-1][players[currentPlayer]->y].isWalkable)
					players[currentPlayer]->x -=1;
			break;
		default:
			// move right
			if(players[currentPlayer]->x < gameMap.w-1)
				if(gameMap.ts[players[currentPlayer]->x+1][players[currentPlayer]->y].isWalkable)
					players[currentPlayer]->x +=1;
			break;
	}
}

void MainGame::loadLevel()
{
	switch(currentLevel)
	{
		case LEVEL_1:
			level = new Level();
			level->graphics = ".\\levels\\level00\\graphicTiles.bmp";
			level->index = ".\\levels\\level00\\index.map";
			level->alphaR = 0xFF;
			level->alphaG = 0xE2;
			level->alphaB = 0xAA;

			// initialize a small set of enemies
			Graphics *monsterGraphics = new Graphics(".\\images\\enemies.png",0xFF, 0x0, 0xFF);

			// skeleton
			monsters[0].graphics = monsterGraphics;
			monsters[0].clip->x = 6*16;
			monsters[0].clip->y = 2*16;
			monsters[0].clip->w = 16;
			monsters[0].clip->h = 16;
			monsters[0].x = 15;
			monsters[0].y = 20;

			// thug
			monsters[1].graphics = monsterGraphics;
			monsters[1].clip->x = 0;
			monsters[1].clip->y = 0;
			monsters[1].clip->w = 16;
			monsters[1].clip->h = 16;
			monsters[1].x = 25;
			monsters[1].y = 10;

			// porcupine
			monsters[2].graphics = monsterGraphics;
			monsters[2].clip->x = 14*16;
			monsters[2].clip->y = 0;
			monsters[2].clip->w = 16;
			monsters[2].clip->h = 16;
			monsters[2].x = 25;
			monsters[2].y = 25;

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

	paintObject(&monsters[0]);
	paintObject(&monsters[1]);
	paintObject(&monsters[2]);
}

void MainGame::paintObject(Object* obj)
{
	if(gameMap.isOnScreen(mainCharacter->x,mainCharacter->y))
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
