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

// Game states
const int STATE_CHARACTER_SELECTION = 0;
const int STATE_MAIN_GAME = 1;

int state;

int main( int argc, char* args[] ) 
{
	// Initialize the SDL library 
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) 
	{ 
		return 1; 
	} 
	
	// Initialize the main graphics screen 
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); 

	// set the window title for the main window
	SDL_WM_SetCaption( WINDOW_TITLE, NULL );




	SelectionScreen selectionScreen = SelectionScreen(screen);

/*
	selectionScreen.setBackground(screen);
	selectionScreen.load();
*/



	//Make sure the program waits for a quit, instead of a timed exit
	bool quit = false;

	while(quit != true) 
	{

		while( SDL_PollEvent( &event ) ) 
		{
			if( event.type == SDL_KEYDOWN ) 
			{
				if (state == STATE_CHARACTER_SELECTION)
				{
					switch( event.key.keysym.sym ) 
					{
						case SDLK_UP:
							//selectionScreen.keyUp();
							break;
						case SDLK_w:
							break; 
					}
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
		}

		switch(state)
		{
			case STATE_CHARACTER_SELECTION:
				selectionScreen.paint();
				break;
			case STATE_MAIN_GAME:
				//mainGame.paint();
				break;
		}

		if( SDL_Flip( screen ) == -1 ) 
		{ 
			return 2; 
		}

		SDL_Delay(FRAME_RATE_SLEEP);
	}
	
	return 0;
}