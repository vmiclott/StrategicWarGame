#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <string>
#include <iostream>

#include "allegro5\allegro.h"
#include "AllegroLib.h"
#include "Color.h"
#include "Grid.h"
#include "constants.h"

//forward declaration.
class World;

class Graphics
{
public:
	static Graphics& Instance()
	{
		static Graphics instance;
		return instance;
	}

	~Graphics() {};

	enum Sprite {
		//We start with all the sprites that do not depend on a theme(e.g. civ_a) in alphabetical order. These are in the civ folder.
		SPRITE_BADGE_AP,
		SPRITE_BADGE_ATTACK,
		SPRITE_BADGE_HP,
		SPRITE_BADGE_RANGE_MAX,
		SPRITE_BADGE_RANGE_MIN,
		SPRITE_CROSS_GREY,
		SPRITE_CROSSHAIR,
		SPRITE_ICON,
		SPRITE_LMB,
		SPRITE_RMB,
		SPRITE_SELECT_HIGHLIGHT,
		SPRITE_TARGET,
		SPRITE_TARGET_BLAST,
		SPRITE_WAIT,
		SPRITE_VICTORY,
		SPRITE_DEFEAT,

		//Then we add all the sprites that depend on a theme like civ_a or civ_b. These are in the civ_a or civ_b folder.	
		SPRITE_ACTOR,
		SPRITE_ARCHERY,
		SPRITE_BRIDGE,
		SPRITE_BRIDGE2,		
		SPRITE_DESTINATION,
		SPRITE_FIRE,
		SPRITE_FLAG1,
		SPRITE_FLAG2,
		SPRITE_HEALTH_MIN,
		SPRITE_HEALTH1,
		SPRITE_HEALTH2,
		SPRITE_HEALTH3,
		SPRITE_HEALTH4,
		SPRITE_HEALTH5,
		SPRITE_HEALTH6,
		SPRITE_HEALTH7,
		SPRITE_HEALTH8,
		SPRITE_HEALTH9,
		SPRITE_HEALTH10,
		SPRITE_HQ,
		SPRITE_HUD,
		SPRITE_INFANTRY,
		SPRITE_PATH,
		SPRITE_PATH_FAR,
		SPRITE_ROAD_0,
		SPRITE_ROAD_1,
		SPRITE_ROAD_2,
		SPRITE_ROAD_3,
		SPRITE_ROAD_4,
		SPRITE_ROAD_5,
		SPRITE_ROAD_6,
		SPRITE_ROAD_7,
		SPRITE_ROAD_8,
		SPRITE_ROAD_9,
		SPRITE_ROAD_10,
		SPRITE_ROAD_11,
		SPRITE_ROAD_12,
		SPRITE_ROAD_13,
		SPRITE_ROAD_14,
		SPRITE_ROAD_15,
		SPRITE_ROCK,
		SPRITE_ROCK2,
		SPRITE_ROCK3,
		SPRITE_SELECT,
		SPRITE_SHIELD,
		SPRITE_TERRAIN,
		SPRITE_TERRAIN2,
		SPRITE_WATER,
		
		//Special sprites.
		SPRITE_NONE,
		SPRITE_WORLD,
		SPRITE_LENGTH
	};

	enum Align {
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT
	};

	void LoadFonts();
	void LoadSpriteCache();
	void UnLoadFonts();
	void UnLoadSpriteCache();

	void ExecuteDraws();
	void ClearScreen();

	void GenerateBackgroundSprite(World* world);

	void DrawBitmap(Sprite sprite, float dx, float dy);
	void DrawString(std::string str, float dx, float dy, Color c, Align align, bool hugeFont);
	void DrawRectangle(float dx, float dy, float width, float height, Color c, float thickness);

	Vector2 ToPx(Grid gridloc);
	Grid FromPx(Vector2 vec);

	int GetGridSize();

    void TestDraw();

private:
	bool redraw;
	AllegroLib* al;
	std::vector<ALLEGRO_BITMAP *> sprites;
	ALLEGRO_FONT* font;
    ALLEGRO_FONT* big_font;

	/* Private constructor to make sure no extra instances are made */
	Graphics(): 
		redraw(true), 
		al(&AllegroLib::Instance()) {
	};

	//Helper methods to work with allegro.
	ALLEGRO_FONT* Load_font(char* font_filefont_file, int size);
	ALLEGRO_BITMAP* Load_bitmap(char* base_name, const char* theme);

	/* Private copy constructor to make sure no extra instances are made */
	Graphics(Graphics const&) {};
	/* Private assignation operator to make sure no extra instances are made */
	void operator=(Graphics const&) {};
	
};

#endif