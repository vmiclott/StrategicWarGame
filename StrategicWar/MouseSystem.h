#ifndef MOUSE_SYSTEM_H
#define MOUSE_SYSTEM_H

#include "System.h"
#include "Engine.h"
#include "AllegroLib.h"
#include "Vector2.h"
#include "Grid.h"
#include "Graphics.h"
#include "UnitComponent.h"
#include "AnimationComponent.h"
#include "SelectionComponent.h"
#include "HoverComponent.h"
#include "Pathfinder.h"

class MouseSystem : public System
{
public:
	MouseSystem();
    ~MouseSystem() {};

	void MouseSystem::SetEngine(Engine* engine);

protected:
	virtual void Update();

	virtual Type GetType() { return System::TYPE_MOUSE; };

private:

	void MouseSystem::SelectUnit(Entity* unit);
	void MouseSystem::DeselectCurrentUnit();

	void Attack(Entity* unit, Vector2 destination, Entity* AiUnit);
	void Move(Entity* unit, Vector2 destination, std::vector<Grid> path);

	//Field to hold the currently selected tile.
	Entity* selectedTile;

	//Field with allegroLib to handle the mouse events.
	AllegroLib* al;

};

#endif