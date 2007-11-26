#include "startScreen.h"


StartScreen::StartScreen(int newX, int newY, int newW, int newH) : Screen(newX,newY,newW,newH)
{
	mapEditor = NULL;
	startMusic = NULL;
	background = NULL;
	state = STATE_SHOW_SCREEN;
	isDone = false;
	init();
}

StartScreen::~StartScreen()
{
	std::cout << "Destroying StartScreen\n";
	if(background != NULL)
		SDL_FreeSurface(background);
	if(mapEditor != NULL)
		mapEditor->~MapEditor();
	if(musicStarted)
		Mix_HaltMusic();
	//if(startMusic != NULL)
	//	Mix_FreeMusic(startMusic);
}

void StartScreen::init()
{
	startMusic = Mix_LoadMUS(".\\music\\start.mp3");
	musicStarted = false;

	background = loadImage(".\\images\\startScreen.png");

	btnStartGame = Rect(14*TILE_SIZE,18*TILE_SIZE,22*TILE_SIZE,3*TILE_SIZE);
	btnCreateNewLevel = Rect(14*TILE_SIZE,22*TILE_SIZE,22*TILE_SIZE,3*TILE_SIZE);
	btnEditLevel1 = Rect(17*TILE_SIZE,26*TILE_SIZE,16*TILE_SIZE,3*TILE_SIZE);
	btnEditLevel2 = Rect(17*TILE_SIZE,30*TILE_SIZE,16*TILE_SIZE,3*TILE_SIZE);
}

void StartScreen::paint()
{
	if(state == STATE_SHOW_SCREEN)
	{
		//start music
		if (!musicStarted)
		{
			Mix_PlayMusic( startMusic,-1 );
			musicStarted=true;
		}

		// apply background image
		applySurface(0,0,background,screen);
	}
	else if(state == STATE_MAP_EDITOR)
	{
		mapEditor->paint();
	}
}

void StartScreen::mouseLeft(int clickX, int clickY)
{
	if(state == STATE_SHOW_SCREEN)
	{
		if(btnStartGame.inBounds(clickX, clickY))
		{
			Mix_HaltMusic();
			musicStarted = false;
			signalCompletion();
		}
		else if(btnCreateNewLevel.inBounds(clickX, clickY))
		{
			showMapEditor(0);
		}
		else if(btnEditLevel1.inBounds(clickX, clickY))
		{
			showMapEditor(1);
		}
		else if(btnEditLevel2.inBounds(clickX, clickY))
		{
			showMapEditor(2);
		}
	}
	else if(state == STATE_MAP_EDITOR)
	{
		mapEditor->mouseLeft(clickX, clickY);
		if(isDone)
		{
			isDone = false;
			state = STATE_SHOW_SCREEN;
		}
	}
}

void StartScreen::mouseRight(int clickX, int clickY)
{
	if(state == STATE_MAP_EDITOR)
	{
		mapEditor->mouseRight(clickX, clickY);
	}
}

void StartScreen::keyW()
{
	if(state == STATE_MAP_EDITOR)
		mapEditor->toggleWalkable();
}

void StartScreen::showMapEditor(int level)
{
	if(state != STATE_MAP_EDITOR)
	{
		if(mapEditor == NULL)
		{
			mapEditor = new MapEditor(x,y,w,h);
			mapEditor->setSignal(&isDone);
		}
		switch(level)
		{
			case 1:
				mapEditor->load(LEVEL_1_MAP, LEVEL_1_GRAPHICS);
				break;
			case 2:
				mapEditor->load(LEVEL_2_MAP, LEVEL_2_GRAPHICS);
				break;
			default:
				mapEditor->load();
				break;
		}
		state = STATE_MAP_EDITOR;
	}
}

