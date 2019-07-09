#ifndef SYSTEM_H
#define SYSTEM_H

#include "Context.h"

// Forward declaration
class Engine;

class System
{
public:
	enum Type {
		TYPE_AI, 
        TYPE_ANIMATION,
        TYPE_GAMESTATE,
        TYPE_KEY,
        TYPE_MOUSE,
        TYPE_RENDER, 
        TYPE_SCORE, 
        TYPE_SELECTION, 
        NUMBER_OF_SYSTEMS
	};

	System() {};
	virtual ~System() {};

	virtual void Update() = 0;
	virtual Type GetType() = 0;

	virtual void SetEngine(Engine* _engine) { engine = _engine; };

protected:
	Engine* engine;
};

#endif