#ifndef GAME_H
#define GAME_H

#include <string>
#include "Engine.h"
#include "Context.h"
#include "RenderSystem.h"
#include "MouseSystem.h"
#include "AnimationSystem.h"
#include "KeySystem.h"
#include "AISystem.h"
#include "ScoreSystem.h"
#include "SelectionSystem.h"
#include "GameStateSystem.h"

class Game
{
public:
	Game(Context* _context) :
		context(_context),
		size(_context->GetScreenSize()),
		engine(_context),
		fps(_context->GetFps()){}
	
	~Game();

	void Init();
	void LoadAssets();
	void LoadWorld(std::string world_file);
	
	void Run();

private:
	// Methods
	void CaptureInput();
	void DestroyOther();
	void DestroyEntities();
	void AddStatEntities();

	// Fields
	double fps;
	Vector2 size;
	Engine engine;
	RenderSystem rs;
	MouseSystem ms;
	AnimationSystem as;
	KeySystem ks;
    AISystem ais;
	ScoreSystem ss;
    SelectionSystem sels;
    GameStateSystem gs;
	Context* context;

	World* world;
	Pathfinder* pathfinder;
};

#endif