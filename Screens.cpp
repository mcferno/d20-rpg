#include "Screens.h"

#include <iostream>

Graphics *characterSprites = NULL;
Graphics *highlightTile = NULL;
Graphics *selectScreen = NULL;
Character *mainCharacter = NULL;


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

const int MainGame::LEVEL_1 = 1;
const int MainGame::STATE_LEVEL_START = 0;
const int MainGame::STATE_HUMAN_TURN = 1;
const int MainGame::STATE_AI_TURN = 2;

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
			break;
	}
}

void MainGame::paint()
{
	gameMap.paint();

	paintObject(mainCharacter);
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
	if(mainCharacter->y > 0)
	{
		if(gameMap.ts[mainCharacter->x][mainCharacter->y-1].isWalkable)
			mainCharacter->y -= 1;
	}
}
void MainGame::keyDown()
{
	if(mainCharacter->y < gameMap.h-1)
	{
		if(gameMap.ts[mainCharacter->x][mainCharacter->y+1].isWalkable)
			mainCharacter->y += 1;
	}
}
void MainGame::keyLeft()
{
	if(mainCharacter->x > 0)
	{
		if(gameMap.ts[mainCharacter->x-1][mainCharacter->y].isWalkable)
			mainCharacter->x -= 1;
	}
}
void MainGame::keyRight()
{
	if(mainCharacter->x < gameMap.w-1)
	{
		if(gameMap.ts[mainCharacter->x+1][mainCharacter->y].isWalkable)
			mainCharacter->x += 1;
	}
}
