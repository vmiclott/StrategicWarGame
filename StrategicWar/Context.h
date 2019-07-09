#ifndef CONTEXT_H
#define CONTEXT_H

#include "Vector2.h"
#include "Grid.h"
#include <math.h>
#include "constants.h"

class World;
class Pathfinder;

class Context
{
public:
	Context(Vector2 _size) :
		size(_size),
		fps(FPS)
	{};
	~Context() {};

	enum GameState {
		PL_ATTACK,
		PL_MOVE,
		PL_PLAY,
		PL_FINISH,
		AI_ATTACK,
		AI_MOVE,
		AI_PLAY,
		AI_FINISH,
		GAME_OVER,
		NUMBER_OF_GAME_STATES
	};

	/* Checks whether the debug_mode flag is active */
	bool IsDebugMode() { return debug_mode; };
	
	/* Sets the debug_mode flag */
	void SetDebugMode(bool _debug_mode) { debug_mode = _debug_mode; };

	/* Returns the screen total size in pixels */
	Vector2 GetScreenSize() { return size; };
	
	/* Returns the fps of the game */
	double GetFps() { return fps; };

	/* Returns the world */
	World* GetWorld() { return world; }

	/* Sets the world */
	void SetWorld(World* w) { world = w; }

	/* Returns the pathfinder */
	Pathfinder* GetPathfinder() { return pathfinder; }

	/* Sets the pathfinder */
	void SetPathfinder(Pathfinder* finder) { pathfinder = finder; }

	/* Returns the current gamestate*/
	GameState GetGameState() { return currentGameState; }

	/* Sets the current gamestate */
	void SetGameState(GameState state) { currentGameState = state; }

private:
	Vector2 size;
	bool debug_mode;
	double fps;
	World* world;
	Pathfinder* pathfinder;
	GameState currentGameState;
};

#endif