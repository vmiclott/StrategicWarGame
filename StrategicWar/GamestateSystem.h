#ifndef GAMESTATE_SYSTEM_H
#define GAMESTATE_SYSTEM_H

#include "System.h"
#include "Engine.h"
#include "Context.h"
#include "UnitComponent.h"
#include "SelectionComponent.h"
#include "PauseComponent.h"

class GameStateSystem : public System
{
public:

    GameStateSystem() :
        System() {};
    ~GameStateSystem() {};


protected:

    virtual void Update();
    virtual Type GetType() { return System::TYPE_GAMESTATE; };

    void EndTurn(Entity::Player player);
    void ClearPausedUnits();
    void DeselectCurrentUnit();
    void ResetUnitsAP();
    void PauseUnitsWithoutAP();
    void PauseUnit(Entity* unit);
    void UnpauseUnit(Entity* unit);
    void GameStateSystem::AddPausedTexture(Entity* entity);
    void GameStateSystem::RemovePausedTexture(Entity* entity);

};

#endif