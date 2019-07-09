#include "Game.h"
#include "AllegroLib.h"
#include "Graphics.h"
#include "Entity.h"
#include "EntityStream.h"
#include "World.h"
#include "Vector2.h"
#include <vector>
#include <iostream>
#include <fstream>


using namespace std;

Game::~Game()
{
	DestroyOther();
	DestroyEntities();
	delete(world);
	delete(pathfinder);
}

/* Start the actual gameloop */
void Game::Run()
{
	AllegroLib::Instance().StartLoop();
	bool quit = AllegroLib::Instance().IsWindowClosed();
	while (!quit)
	{
		// Fetch event
		AllegroLib::Instance().StartIteration();

		// Update all Systems
		engine.Update();

		CaptureInput(); // #TODO wat doet deze functie? nog nodig?


		// Check for window close
		quit = AllegroLib::Instance().IsWindowClosed();

		//Check for victory
		ScoreComponent* sc = dynamic_cast<ScoreComponent*>((*(engine.GetEntityStream()->WithTag(Component::SCORE)).begin())->GetComponent(Component::SCORE));
		if (!(engine.GetContext()->GetGameState() == Context::GAME_OVER) && (sc->aiWon || sc->playerWon)) {
			engine.GetContext()->SetGameState(Context::GAME_OVER);
			Entity* entity = new Entity();
			TextureComponent* text;
			if (sc->aiWon) {
				text = new TextureComponent({Graphics::SPRITE_DEFEAT});
			}
			else {
				text = new TextureComponent({ Graphics::SPRITE_VICTORY });
			}
			entity->Add(text);
			entity->Add(new PositionComponent(Grid(0, 0),2));
			engine.AddEntity(entity);
		}
	}
}

/* Inititalise allegro and show the display. */
void Game::Init()
{
	AllegroLib::Instance().Init(size, fps);
}

/* Loads sprites, fonts and adds systems to the Engine */
void Game::LoadAssets()
{
	Graphics::Instance().LoadSpriteCache();
	Graphics::Instance().LoadFonts();

	// TODO: Add all required systems
    // #TODO systems bijhouden in een lijst om het risico op bugs vermindert
    // (vb vergeten te verwijderen in Game::DestroyOther())
	engine.AddSystem(&rs);  // Renders all to screen
    engine.AddSystem(&ais); // Add the AISystem
	engine.AddSystem(&ms);  // Add the MouseSystem
	engine.AddSystem(&ks); // Add the KeySystem
    engine.AddSystem(&sels); // Add the SelectionSystem
	engine.AddSystem(&as); // Add the AnimationSystem
	engine.AddSystem(&ss); // Add the Scoresystem
    engine.AddSystem(&gs); // Add the GameStateSystem

	//add entities to display stats
	AddStatEntities();
}

/* Load the level from file */
void Game::LoadWorld(string world_file)
{
	//Create the world.
	world = new World();
	world->Init(world_file, &engine);
	engine.GetContext()->SetWorld(world);

	//Create the pathfinder
	pathfinder = new Pathfinder(world, engine.GetEntityStream());
	engine.GetContext()->SetPathfinder(pathfinder);
	Graphics::Instance().GenerateBackgroundSprite(world);

	//Set the starting gamestate
	engine.GetContext()->SetGameState(Context::PL_PLAY);
}

/* Captures input */
void Game::CaptureInput()
{
	AllegroLib* al = &AllegroLib::Instance();
}

void Game::DestroyEntities()
{
	// TODO: Destroy all Entities still attached to the Engine.
}

/* Destroys everything else */
void Game::DestroyOther()
{
	Graphics::Instance().UnLoadFonts();
	Graphics::Instance().UnLoadSpriteCache();

	// Remove systems
	engine.RemoveSystem(&rs);
	engine.RemoveSystem(&ms);
	engine.RemoveSystem(&as);
	engine.RemoveSystem(&ks);
    engine.RemoveSystem(&ais);
	engine.RemoveSystem(&ss);
    engine.RemoveSystem(&sels);
    engine.RemoveSystem(&gs);

	AllegroLib::Instance().Destroy();
}

void Game::AddStatEntities() {
	std::vector<Entity*> entities;
	entities.resize(5);
	

	for (int i = 0; i < 5; i++) {
		entities[i] = new Entity();
		entities[i]->Add(new PositionComponent(Grid(13+i, 1), 0));
	}

	entities[0]->Add(new TextureComponent({ Graphics::SPRITE_BADGE_HP }));
	entities[1]->Add(new TextureComponent({ Graphics::SPRITE_BADGE_AP }));
	entities[2]->Add(new TextureComponent({ Graphics::SPRITE_BADGE_ATTACK }));
	entities[3]->Add(new TextureComponent({ Graphics::SPRITE_BADGE_RANGE_MIN }));
	entities[4]->Add(new TextureComponent({ Graphics::SPRITE_BADGE_RANGE_MAX }));
	for (int i = 0; i < 5; i++) {
		engine.AddEntity(entities[i]);
	}

}