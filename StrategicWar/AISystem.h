#ifndef AI_SYSTEM_H
#define AI_SYSTEM_H

#include "System.h"
#include "Engine.h"
#include "AllegroLib.h"

class AISystem : public System
{
public:

    AISystem() :
        System() {};
    ~AISystem() {};


protected:

    virtual void Update();
    virtual Type GetType() { return System::TYPE_AI; };


private:

    Entity* FindFirstMovableUnit();
    void HandleAILogic(Entity* selectedUnit);
    Entity* FindNearestHumanPlayerUnit(Entity* selectedUnit);
    bool AttackEnemy(Entity* selectedUnit, Entity* enemy);
    bool MoveUnitTowardsEntity(Entity* selectedUnit, Entity* entity);
    bool MoveUnitTowardsEnemy(Entity* selectedUnit, Entity* enemy);
    bool MoveUnitAwayFromEnemyUntilOutsideMinRange(Entity* selectedUnit, Entity* enemy);
    void SelectUnit(Entity* unit);
    void SelectTarget(Vector2 pos);
    std::set<Entity*> FindUnitsByPlayer(Entity::Player owner); // #TODO move to more appropriate class
    std::set<Entity*> FindEnemiesInsideRange(Entity* selectedUnit, unsigned int range); // #TODO move to more appropriate class
    std::set<Entity*> FindEnemiesInsideMaxRange(Entity* selectedUnit); // #TODO move to more appropriate class
    std::set<Entity*> FindEnemiesInsideMinRange(Entity* selectedUnit); // #TODO move to more appropriate class
    std::set<Entity*> FilterEnemiesInRange(Entity* selectedUnit, std::set<Entity*> enemies);
    Entity* FilterNearestUnit(Entity* selectedUnit, std::set<Entity*> units);
    Entity* FilterNearestEntity(Entity* selectedUnit, std::set<Entity*> entities);
    std::set<Entity*> FindEntitiesAtDistanceAroundUnit(Entity* unit, unsigned int distance);


};

#endif
