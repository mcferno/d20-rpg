#include "Graphics.h"
#include "Screens.h"

//Screen attributes 
const int SCREEN_WIDTH = 800; 
const int SCREEN_HEIGHT = 600; 
const int SCREEN_BPP = 32;
const char* WINDOW_TITLE = "COMP446: Final Project";
const int FRAME_RATE_SLEEP = 40;

//The event structure that will be used 
SDL_Event event;

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
Uint32 bgColor;

SelectionScreen selectionScreen;
MainGame mainGame;

// Game states
const int STATE_CHARACTER_SELECTION = 0;
const int STATE_MAIN_GAME = 1;

int state = STATE_CHARACTER_SELECTION;

void paint()
{
	// paint the background black
	SDL_FillRect(screen, NULL, bgColor);

	switch(state)
	{
		case STATE_CHARACTER_SELECTION:
			selectionScreen.paint();
			break;
		case STATE_MAIN_GAME:
			mainGame.paint();
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
	
	// Initialize the main graphics screen 
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); 

	bgColor = SDL_MapRGB( screen->format, 0, 0, 0 );
	
	// set the window title for the main window
	SDL_WM_SetCaption( WINDOW_TITLE, NULL );

	selectionScreen = SelectionScreen(screen);

	mainGame = MainGame(screen);

/*
	selectionScreen.setBackground(screen);
	selectionScreen.load();
*/

	// initial paint
	paint();

	//Make sure the program waits for a quit, instead of a timed exit
	bool quit = false;

	while(quit != true) 
	{

		while( SDL_PollEvent( &event ) ) 
		{
			if( event.type == SDL_KEYDOWN ) 
			{
				switch( event.key.keysym.sym ) 
				{
					case SDLK_UP:
						if(state == STATE_MAIN_GAME)
							mainGame.keyUp();
						break;
					case SDLK_DOWN:
						if(state == STATE_MAIN_GAME)
							mainGame.keyDown();
						break;
					case SDLK_LEFT:
						if(state == STATE_MAIN_GAME)
							mainGame.keyLeft();
						break;
					case SDLK_RIGHT:
						if(state == STATE_MAIN_GAME)
							mainGame.keyRight();
						break;
					case SDLK_q:
						// temporary key used to jump to the Main Game mode (for testing purposes)
						state = STATE_MAIN_GAME;
						mainGame.init();
						break; 
				}
			}
			
			if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				if( event.button.button == SDL_BUTTON_LEFT )
				{
					if (state == STATE_CHARACTER_SELECTION)
						selectionScreen.mouseLeft(event.button.x,event.button.y);

				}
				if( event.button.button == SDL_BUTTON_RIGHT )
				{
					if (state == STATE_CHARACTER_SELECTION)
						selectionScreen.mouseRight(event.button.x,event.button.y);
				}
			}

			//if(selectionScreen.isDone())
			//{
				//mainScreen.initCharacter(selectionScreen.chosenGraphics);
				//state = STATE_MAIN_GAME;
				// destroy selectionscreen.
			//}
			


			if( event.type == SDL_QUIT ) 
			{ 
				quit = true; 
			} 

			// paint only if an event occured
			paint();
		}

		SDL_Delay(FRAME_RATE_SLEEP);
	}
	
	return 0;
}