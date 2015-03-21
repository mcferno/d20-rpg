#ifndef CONFIG_H
	#define CONFIG_H

// stores constants used across a great number of classes

// the fixed size of the tiles
static const int TILE_SIZE = 16;

// conversion between feet and tile size (5ft per tile)
static const int FT_TO_TILES = 5;

/**
 * Image paths to in-app assets
 */

static const char* ITEM_GRAPHICS = "/images/items.png";

static const char* LEVEL_1_MAP = "/levels/level01/index.map";
static const char* LEVEL_1_GRAPHICS = "/levels/level01/graphicTiles.png";

static const char* LEVEL_2_MAP = "/levels/level02/index.map";
static const char* LEVEL_2_GRAPHICS = "/levels/level02/graphicTiles.png";

static const char* START_SCREEN_BACKDROP = "/images/startScreen.png";

static const char* EQUIPMENT_SCREEN_BACKDROP = "/images/equipScreen.png";
static const char* EQUIPMENT_SCREEN_BUTTONS = "/images/equipButton.png";

static const char* MAIN_SCREEN_BACKDROP_DEFAULT = "/images/mainScreen.png";
static const char* MAIN_SCREEN_BACKDROP_WIN = "/images/winScreen.png";
static const char* MAIN_SCREEN_BACKDROP_LOSE = "/images/loseScreen.png";

static const char* EDITOR_SCREEN_BACKDROP = "/images/mapEditorBg.png";
static const char* SELECT_SCREEN_BACKDROP = "/images/selectScreen.png";
static const char* SHOP_SCREEN_BACKDROP = "/images/shopScreen.png";

static const char* UI_NAV_ARROWS = "/images/arrows.png";
static const char* UI_IN_GAME_BUTTONS = "/images/mainGameButtons.png";
static const char* UI_SHOP_BUTTONS = "/images/buySellButtons.png";

static const char* TILE_EMPTY = "/images/emptycell.png";
static const char* TILE_HIGHLIGHTED = "/images/highlight.png";
static const char* TILE_WALKABLE = "/images/fullHighlight.png";
static const char* TILE_ATTACKABLE = "/images/fullHighlightWeapon.png";

static const char* SPRITES_AI = "/images/enemies.png";
static const char* SPRITES_CHESTS = "/images/treasure.png";
static const char* SPRITES_MAP_OBJECTS = "/images/defaultTiles.png";
static const char* SPRITES_HERO = "/images/characters.png";

/**
 * Font assets
 */

static const char* FONT_REGULAR = "/fonts/calibri.ttf";
static const char* FONT_BOLD = "/fonts/calibrib.ttf";

/**
 * Music assets
 */

static const char* MUSIC_INTRO = "/music/start.mp3";

#endif
