#ifndef SELECTION_SYSTEM_H
#define SELECTION_SYSTEM_H

#include "System.h"
#include "Engine.h"
#include "SelectionComponent.h"
#include "UnitComponent.h"
#include "Pathfinder.h"
#include "Grid.h"

class SelectionSystem : public System
{
public:

    SelectionSystem() :
        System() {};
    ~SelectionSystem() {};


protected:

    virtual void Update();
    virtual Type GetType() { return System::TYPE_SELECTION; };
	void SetEngine(Engine* engine);


private:

    Grid lastHoveredPos;
    std::vector<Entity*> currentPath;
	Entity* lastHoveredEntity;

    Entity* FindSingleSelectedEntity(Component::Tag tag); // there can only be one selected unit / tile
    void AddSelectionTexture(Entity* unit);
    void RemoveSelectionTexture(Entity* unit);
    Graphics::Sprite SelectionSystem::GetSelectionSpriteForEntity(Entity* entity);
    void CreateCurrentPath(Entity * unit, Entity * tile);
    void ClearCurrentPath();
    Entity* FindUnitOnTile(Entity* tile);


};
#endif