#ifndef SCREEN_H
	#define SCREEN_H

#include "Graphics.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Sprites.h"
#include "Items.h"
#include "Inventory.h"

/* Class Screen: A generic screen which can be painted to the monitor. Has a 
 *   rectangular shape which is defined in pixel
 */
class Screen : public Rect
{
protected:
	// pointer to the main screen, heavily reused for painting
	static SDL_Surface *screen;

	// a tile used to indicate that something has been selected
	static SDL_Surface *highlightTile;

	// various fonts used any time there it text rendered to the screen
	static TTF_Font *fontCalibri, *fontCalibriBold, *fontCalibriTiny;
	static SDL_Color colorWhite, colorBlack;

	// the chosen character, shared amongst all screens
	static ControllableCharacter *mainCharacter;

	// a 'flag' to allow a child window to notify its parent
	bool* signal;

	// notifies the parent window that the current window wishes to close
	void signalCompletion();
public:
	Screen(int,int,int,int);
	virtual ~Screen();
	virtual void paint() = 0;

	// initializes the static variables used by all derived classes
	static void init(SDL_Surface *);

	// sets the pointer to the signal that will be used later by signalCompletion()
	void setSignal(bool *);
};

#endif