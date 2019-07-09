#include "Graphics.h"
#include "Entity.h"
#include "World.h"

using namespace std;

Vector2 Graphics::ToPx(Grid gridloc)
{
	int grid_size = GetGridSize();
	return Vector2(gridloc.col * grid_size, gridloc.row * grid_size);
}

Grid Graphics::FromPx(Vector2 vec)
{
    int grid_size = GetGridSize();
	return Grid(vec.y / grid_size, vec.x / grid_size);
}

int Graphics::GetGridSize()
{
	return GRID_SIZE;
}

void Graphics::LoadFonts()
{
	font = Load_font(FONT_ARIAL, FONT_SIZE_SMALL);
	big_font = Load_font(FONT_ARIAL, FONT_SIZE_HUGE);
}

void Graphics::LoadSpriteCache()
{
	char* theme = "civ_a";

	sprites.resize(SPRITE_LENGTH);

	//Array with all the filenames of the general sprites (=sprites that don't depend on the chosen theme, in the civ folder).
	//The filenames are in alphabetical order. This is important to connect the loaded bitmap to the correct value from the enum.
	char* generalFileNames[] = { "badge_ap", "badge_attack", "badge_hp","badge_range_max", "badge_range_min",
		"cross_grey", "crosshair", "icon", "lmb", "rmb", "select_highlight", "target", "target_blast", "wait", "victory", "defeat" };

	for (int index = SPRITE_BADGE_AP; index < SPRITE_ACTOR; index++) {
		sprites[index] = Load_bitmap(generalFileNames[index], "civ");
	}

	//Array with all the filenames of sprites that depend on the theme(in civ_a or civ_b folder).
	char* specificFileNames[] = { "actor", "archery", "bridge", "bridge2", "destination", "fire", "flag1", "flag2", "health0.5", "health1",
		"health2", "health3", "health4", "health5", "health6", "health7", "health8", "health9", "health10", "hq", "hud","infantry", "path", "path_far",
		"road_0", "road_1", "road_2", "road_3", "road_4", "road_5", "road_6", "road_7", "road_8", "road_9", "road_10", "road_11", "road_12", "road_13",
		"road_14", "road_15", "rock", "rock2", "rock3", "select", "shield", "terrain", "terrain2", "water" };

	for (int index = SPRITE_ACTOR; index < SPRITE_NONE; index++) {
		sprites[index] = Load_bitmap(specificFileNames[index - SPRITE_ACTOR], theme);
	}

	//Special sprites
	sprites[SPRITE_NONE] = Load_bitmap("actor", theme);
	sprites[SPRITE_WORLD] = NULL;
}

void Graphics::UnLoadFonts()
{
	al_destroy_font(font);
	al_destroy_font(big_font);
}

void Graphics::UnLoadSpriteCache() {
	for (int index = SPRITE_BADGE_AP; index < SPRITE_LENGTH; index++) {
		al_destroy_bitmap(sprites[index]);
	}
}

/* Small function that converts our own Graphics::Align enum to allegro enums */
int getAlign(Graphics::Align align)
{
	int a;
	switch (align) {
	case Graphics::ALIGN_LEFT:
		a = ALLEGRO_ALIGN_LEFT;
		break;
	case Graphics::ALIGN_CENTER:
		a = ALLEGRO_ALIGN_CENTRE;
		break;
	case Graphics::ALIGN_RIGHT:
		a = ALLEGRO_ALIGN_RIGHT;
		break;
	}
	return a;
}

void Graphics::ExecuteDraws()
{
	al_flip_display();
}

void Graphics::ClearScreen()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
}

void Graphics::GenerateBackgroundSprite(World * world)
{
	// Create an appropriately sized bitmap for the SPRITE_WORLD bitmap pointer and set this bitmap as target for draw calls.
	int grid_size = GetGridSize();

	ALLEGRO_BITMAP* level = al_create_bitmap(world->GetWidth() * grid_size, world->GetHeight() * grid_size);
	al_set_target_bitmap(level);

	// Draw all segments of the background (level)
	for (Entity* &entity : world->GetGroundTiles())
	{
		if (entity->GetComponent(Component::POSITION) != NULL
			&& entity->GetComponent(Component::TEXTURE) != NULL)
		{
			PositionComponent* pc = dynamic_cast<PositionComponent*>(entity->GetComponent(Component::POSITION));
			TextureComponent* tc = dynamic_cast<TextureComponent*>(entity->GetComponent(Component::TEXTURE));
			Vector2 coordinates = pc->pos;
			vector<Graphics::Sprite> textures = tc->textures;
			for (Graphics::Sprite texture : textures)
			{
				DrawBitmap(texture, coordinates.x, coordinates.y);
			}
		}
	}

	sprites[SPRITE_WORLD] = level;

	// Reset the target for draw calls to the backbuffer of the display
	al_set_target_backbuffer(al->display);
}

void Graphics::DrawBitmap(Sprite sprite, float dx, float dy)
{
	al_draw_bitmap(sprites[sprite], dx, dy, 0);
}

void Graphics::DrawString(string str, float dx, float dy, Color c, Align align, bool hugeFont)
{
	if (hugeFont) {
		al_draw_text(big_font, al_map_rgb(c.r, c.g, c.b), dx, dy, getAlign(align), str.c_str());
	}
	else {
		al_draw_text(font, al_map_rgb(c.r, c.g, c.b), dx, dy, getAlign(align), str.c_str());
	}
}

void Graphics::DrawRectangle(float dx, float dy, float width, float height, Color c, float thickness)
{
	al_draw_rectangle(dx, dy, dx + width, dy - height, al_map_rgb(c.r, c.g, c.b), thickness);
}

ALLEGRO_FONT* Graphics::Load_font(char* font_file, int size)
{
	ALLEGRO_PATH * path = al_create_path(FILES_ASSETS_PATH);
	al_append_path_component(path, FILES_FONTS_SUBDIRNAME);
	al_set_path_filename(path, font_file);

	const char* filename = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

	ALLEGRO_FONT* font = al_load_ttf_font(filename, size, 0);
	if (font == NULL) {
		cerr << "Failed to load font from " << filename;
		al_destroy_path(path);
		exit(-1);
	}
	al_destroy_path(path);

	return font;
}

ALLEGRO_BITMAP* Graphics::Load_bitmap(char* base_name, const char* theme)
{
	string filename = string(FILES_ASSETS_PATH) + string(FILES_IMAGES_SUBDIRNAME) + "/" + theme + "/" + base_name + ".png";

	ALLEGRO_BITMAP* res = al_load_bitmap(filename.c_str());
	if (res == NULL) {
		cerr << "Failed to load " << base_name << " from " << filename;
		exit(-1);
	}

	return res;
}


void Graphics::TestDraw()
{
	DrawBitmap(SPRITE_WORLD, 0, 0);
	ExecuteDraws();
}