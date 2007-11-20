#include "SDL.h"
#include "Screens.h"
#include "SelectionScreen.h"
#include "MainGame.h"
#include <time.h>

static const int SCREEN_WIDTH = 800; 
static const int SCREEN_HEIGHT = 600; 
static const int SCREEN_BPP = 32;
static const char* WINDOW_TITLE = "COMP446: Final Project";
const int FRAME_RATE_SLEEP = 40;

//The event structure that will be used 
SDL_Event event;

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
Uint32 bgColor;

// changed to pointers since the objects were being created statically
// at run time, which lead to problems since these objects depend on
// the screen having been created first
SelectionScreen *selectionScreen;
MainGame *mainGame;

// Game states
const int STATE_CHARACTER_SELECTION = 0;
const int STATE_MAIN_GAME = 1;
const int STATE_MONSTER_MOVE = 2;

int state = STATE_CHARACTER_SELECTION;

void clearScreen()
{
	// paint the background black
	SDL_FillRect(screen, NULL, bgColor);
}

void paint()
{
	clearScreen();

	switch(state)
	{
		case STATE_CHARACTER_SELECTION:
			selectionScreen->paint();
			break;
		case STATE_MAIN_GAME:
			mainGame->paint();
			break;
	}

	if( SDL_Flip( screen ) == -1 ) 
	{ 
		return; 
	}
}

int main( int argc, char* args[] ) 
{
	// Initialize the SDL library 
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) 
	{ 
		return 1; 
	} 

	if (TTF_Init() == -1)
	{
		return 1;
	}
	
	// Initialize the main graphics screen 
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); 

	Screen::init(screen);

	bgColor = SDL_MapRGB( screen->format, 0, 0, 0 );
	
	// set the window title for the main window
	SDL_WM_SetCaption( WINDOW_TITLE, NULL );

	// seed rand for later use in the game
	srand ((unsigned int)time(NULL));

	bool isDone = false;
	selectionScreen = new SelectionScreen(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	selectionScreen->setSignal(&isDone);

	mainGame = new MainGame(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

	// initial paint
	paint();

	//Make sure the program waits for a quit, instead of a timed exit
	bool quit = false;

	// whether or not the screen needs repainting
	bool dirty = false;

	while(quit != true) 
	{

		while( SDL_PollEvent( &event ) ) 
		{
			if( event.type == SDL_KEYDOWN ) 
			{
				dirty = true;
				switch( event.key.keysym.sym ) 
				{
					case SDLK_s:
						if(state == STATE_MAIN_GAME)
							mainGame->showShop();
						break;
					case SDLK_e:
						if(state == STATE_MAIN_GAME)
							mainGame->showEquipScreen();
						break;
					case SDLK_UP:
						if(state == STATE_MAIN_GAME)
							mainGame->keyUp();
						break;
					case SDLK_DOWN:
						if(state == STATE_MAIN_GAME)
							mainGame->keyDown();
						break;
					case SDLK_LEFT:
						if(state == STATE_MAIN_GAME)
							mainGame->keyLeft();
						break;
					case SDLK_RIGHT:
						if(state == STATE_MAIN_GAME)
							mainGame->keyRight();
						break;
				}
			}
			
			if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				dirty = true;
				if( event.button.button == SDL_BUTTON_LEFT )
				{
					if (state == STATE_CHARACTER_SELECTION)
						selectionScreen->mouseLeft(event.button.x,event.button.y);
					if(state == STATE_MAIN_GAME)
						mainGame->mouseLeft(event.button.x,event.button.y);

				}
				if( event.button.button == SDL_BUTTON_RIGHT )
				{
					if (state == STATE_CHARACTER_SELECTION)
						selectionScreen->mouseRight(event.button.x,event.button.y);
					if(state == STATE_MAIN_GAME)
						mainGame->mouseRight(event.button.x,event.button.y);
				}
			}

			if( event.type == SDL_QUIT ) 
			{ 
				quit = true; 
			} 

			// if the selection screen has signaled it is done, start the game
			if(isDone)
			{
				state = STATE_MAIN_GAME;
				clearScreen();
				mainGame->init();
				isDone = false;
			}

			// paint only if an event occured
			if(dirty)
			{
				paint();
				dirty = false;
			}
		}

		if(state == STATE_MAIN_GAME)
		{
			// send the main game ticks to move game logic (ex: AI)
			mainGame->tick();
		}

		// momentary sleep to avoid using too much CPU
		SDL_Delay(FRAME_RATE_SLEEP);
	}
	
	return 0;
}