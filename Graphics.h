#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <iostream>
#include <fstream>

// A pointer to main graphics screen. Used in painting functions.
extern SDL_Surface *screen;

/* Default initialization value in this library for integers that have not been 
 * given meaningful values.
 */
const int GRAPHICS_INT_DEFAULT = -1;

/* loadImage: opens an image file from the disk. Once opened, the image is 
 *   optimized to match the bit-depth of the graphics screen. Optionally, a
 *   certain color can be masked out in order to help with graphic overlaps.
 *   Returns pointer to the optimized version of the image. 
 *
 *   Supported image formats:
 *     BMP, PNG, JPEG, GIF, TIF, PCX, and more..
 */
SDL_Surface *loadImage( char *, int = -1, int = -1, int = -1);

/* applySurface: applies an image onto a surface (which may also be an image or
 *   or a solid color). Can allow only a certain portion of an image to be 
 *   applied, which is useful when loading large sets of images combined in one
 *   image file
 */
void applySurface(int, int, SDL_Surface*, SDL_Surface*, SDL_Rect* = NULL);

// #####################################################################################################

/* Class Tile: A simple tile, used by the map class to track a single tile in a 
 *   2-dimensional playing field. A tile is assigned graphics for its foreground
 *   and background layers (incase the foreground layer doesn't occupy the 
 *   entire tile surface). A tile is either walkable or not, as defined by the
 *   map designer at creation time.
 */
class Tile
{
public:
	/* Whether or not a tile can be walked on. A non-walkable tile represents
	 *   a stationary object which can not be passed. For example, a wall tile
	 *   is non-walkable. This does not reflect whether or not an actionable
	 *   item is present on the tile (ex: a treasure chest, enemy sprite).
	 */
	bool isWalkable;

	/* Index of the foreground image (top layer). Corresponds to a valid entry 
	 *   in the array of graphic tiles.
	 */
	int f;

	/* Index of the background image (bottom layer). Corresponds to a valid 
	 *   entry in the array of graphic tiles.
	 */
	int b;

	Tile();

	// at minimum, an index for the background image is needed
	Tile(int, int, bool);
};

// #####################################################################################################

/* Class Rect: A simple data structure representing a square with no fixed 
 *   interpretation of what units the x,y,wh are expressed in.
 *   Ex: x = 300px or x = 20 tiles ...
 */
class Rect
{
public:
	// the coordinate of the top left corner of the rectangle
	int x,y;
	
	// the lengths defining the size of the rectangle
	int w,h;

	/* Rect: default construtor initializing all variables to GRAPHICS_INT_DEFAULT
	 */
	Rect();
};

// #####################################################################################################

/* Class Graphics: Represents an image which is a combination of smaller images
 *   all relatively the same size. This image is loaded as one file, but is then
 *   logically cut up into the appropriate pieces so that individual sections of
 *   the image can be used at one time. This class removes the need to have 
 *   seperate files for every image. The smaller images are assumed to be square
 *   and are all of the same size.
 *
 *   The inherited x,y,w,h (Rect) represent the loaded image in pixels
 */
class Graphics : public Rect
{
private:
	/* chopUp: Cuts the loaded image into its individual graphic tiles.
	 */
	void chopUp();

public:
	// the large image containing all the smaller images
	SDL_Surface *image;

	/* the rectangles which define the relative x,y and w,h for all individual 
	 *   tiles, allowing an individual tile to be painted.
	 */
	SDL_Rect *clip;

	// width and height of all tiles
	int tileSize;
	
	// the width and height of the image in tiles
	int wTiles, hTiles;
	
	// the total number of tiles found in the image
	int totalTiles;

	Graphics();
	Graphics(char *, int = 0x0, int = 0xFF, int = 0xFF, int = 16);
	~Graphics();
};

// #####################################################################################################

/* 
 * Class Map: A collection of tiles together used to make a map (grid). The map 
 *   is displayed within a fixed size window on the screen. If the map surpasses
 *   the size of the window bounding it, it will allow scrolling. The map is 
 *   created by loading an index file, which describes every tile on the map
 *   using a Grapics (object). 
 *
 *   Inherited Rect value will represent the size of the map in Tiles.
 *     x,y: index of the top left tile to display within the bounding window
 * 
 *   The format of the index file is a set of integers and characters. The 
 *     width and height of the map are given (in tiles), followed by a line for
 *     every tile in the map.
 *
 *   Example; A 3x2 map index.
 *     3       // the width of the map in tiles
 *     2       // the height of the map in tiles
 *     1 3 Y   // tile[0][0]: background = 1, foreground = 3, walkable
 *     5 9 Y   // tile[0][1]: background = 5, foreground = 9, walkable
 *     3 7 N   // tile[0][2]: background = 3, foreground = 7, not-walkable
 *     2 6 Y   // tile[1][0]: background = 2, foreground = 6, walkable
 *     1 8 Y   // tile[1][1]: background = 1, foreground = 8, walkable
 *     6 7 Y   // tile[1][2]: background = 6, foreground = 7, walkable
 *
 */
class Map : public Rect
{
protected:
	// how many tiles will be shown at one time, the smaller of the map size 
	//   vs. the bounding window size
	int limitWTiles;
	int limitHTiles;

	// 2-dimensional TileSet representing the game map (once loaded).
	Tile **ts;

	// Pointer to the game map graphics
	Graphics *g;

	int tileSize; //, wTiles, hTiles;

public:
	// Dimensions in pixels of the window bounding the map on the screen
	Rect limit;

	// open a *.map file (indexFile), initializing the tileSet
	virtual void loadGraphics(Graphics *);

	// open a *.map file (indexFile), initializing the tileSet
	void loadMap(Graphics *, char *);

	Map();

	// map is created, passing the specs of its bounding window
	Map(int, int, int, int);

	/* parseIndex: opens a file containing the specifics about how a level is 
	 *   layed out according to a fixed format and initializes the set of
	 *   tiles which will be used to recreate the map on the screen.
	 *
	 *   filename: relative or absolute path to the map file
	 *     ex: ".\\storedMaps\\index.map" or "C:\\index.map"
	 */
	void parseIndex(char *);

	/* scrollLeft, scrollRight, scrollUp, scrollDown: moves the tiles within the 
	 *   bounding window. Allows maps larger than the bounding window to be displayed.
	 */
	void scrollLeft();
	void scrollRight();
	void scrollUp();
	void scrollDown();

	/* isOnScreen: Checks whether or not a tile is within the visible window limit.
	 *
	 *   x,y = index of the tile to verify
	 */
	bool isOnScreen(int, int);

	/* paint: Paints all tiles for the map which fall inside the visible window limit.
	 */
	virtual void paint();

	/* paintTile: paints a specific tile on the screen
	 * 
	 *   x,y = the index of the tile to be painted
	 *   locX,locY = the location to paint the tile. If not provided, it will 
	 *     paint in its relative location on the map
	 */
	virtual void paintTile(int, int, int = -1, int = -1);

	/* getTile: gets a pointer to a tile. NULL is returned if the cell is outside the
	 *   range of valid indexes
	 *
	 *   x,y = the index of the tile to return
	 */
	Tile* getTile(int,int);
};

// #####################################################################################################
#endif