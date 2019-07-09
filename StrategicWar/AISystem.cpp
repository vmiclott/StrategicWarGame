#include "AISystem.h"
#include "UnitComponent.h"
#include "GroundtypeComponent.h"
#include "AnimationComponent.h"
#include "SelectionComponent.h"
#include "Pathfinder.h"

#include <algorithm>
#include <iterator>
#include <limits>

using namespace std;


void AISystem::Update()
{
    Entity* selectedUnit = nullptr;

    // if it is the AI's turn
    if (engine->GetContext()->GetGameState() == Context::AI_PLAY)
    {
        selectedUnit = FindFirstMovableUnit();

        if (selectedUnit == nullptr)
        {
            // AI's turn is finished
            engine->GetContext()->SetGameState(Context::AI_FINISH);
        }
        else
        {
            SelectUnit(selectedUnit);
            HandleAILogic(selectedUnit);
        }
    }
}


Entity* AISystem::FindFirstMovableUnit()
{
    Entity* selectedUnit = nullptr;

    // iterate over all AI units
    set<Entity*> aiUnits = FindUnitsByPlayer(Entity::AI); // #TODO dit ergens opslaan zodat niet steeds opnieuw gezocht moet worden? 

    // find first movable unit that still has AP
    for (Entity* unit : aiUnits)
    {
        UnitComponent* uc = dynamic_cast<UnitComponent*>(unit->GetComponent(Component::UNIT));

        if (uc->type != Entity::HQ && uc->ap > 0)
        {
            selectedUnit = unit;
            break;
        }
    }

    return selectedUnit;
}


void AISystem::HandleAILogic(Entity* selectedUnit)
{
    if (selectedUnit != nullptr)
    {
        UnitComponent* uc = dynamic_cast<UnitComponent*>(selectedUnit->GetComponent(Component::UNIT));
        set<Entity*> enemiesInsideMaxRange = FindEnemiesInsideMaxRange(selectedUnit);

        if (enemiesInsideMaxRange.size() > 0)
        {
            set<Entity*> enemiesInRange = FilterEnemiesInRange(selectedUnit, enemiesInsideMaxRange);

            if (enemiesInRange.size() > 0)
            {
                Entity* enemy = FilterNearestUnit(selectedUnit, enemiesInRange);
				if (!enemy) {
					//no enemy that is in range can be reached by the pathfinder
					//so just attack the first one in the set
					enemy = *enemiesInRange.begin();
				}
				if (!AttackEnemy(selectedUnit, enemy))
				{
					// unit could not attack: set its AP to 0 so the AI's turn can end
					uc->ap = 0;
				}
            }
            else
            {
                set<Entity*> enemiesInsideMinRange = FindEnemiesInsideMinRange(selectedUnit);

                if (enemiesInsideMinRange.size() > 0)
                {
                    Entity* enemy = FilterNearestUnit(selectedUnit, enemiesInsideMinRange);

                    if (!MoveUnitAwayFromEnemyUntilOutsideMinRange(selectedUnit, enemy))
                    {
						// unit could not be moved: set its AP to 0 so the AI's turn can end
						uc->ap = 0;
                    }
                }
                else
                {
                    // this shouldn't happen... but set the units AP to 0 so the AI's turn can end
                    uc->ap = 0;
                }
            }
        }
        else
        {
            Entity* nearestEnemy = FindNearestHumanPlayerUnit(selectedUnit);

            if (nearestEnemy == nullptr)
            {
                // no nearest enemy could be found (probably because something's in the way and a path could not be found)
                // set the units AP to 0 so the AI's turn can end
                // #TODO dit intelligenter aanpakken... (op eigen hoofdkwartier afgaan?)
                uc->ap = 0;
            }
            else
            {
                if (!MoveUnitTowardsEnemy(selectedUnit, nearestEnemy))
                {
					// unit could not be moved: set its AP to 0 so the AI's turn can end
					uc->ap = 0;
                }
            }
        }
    }
}


Entity* AISystem::FindNearestHumanPlayerUnit(Entity* selectedUnit)
{
    return FilterNearestUnit(selectedUnit, FindUnitsByPlayer(Entity::HUMAN));
}


bool AISystem::AttackEnemy(Entity* selectedUnit, Entity* enemy)
{
    World* world = engine->GetContext()->GetWorld();
    bool success;

    engine->GetContext()->SetGameState(Context::AI_ATTACK);

    if (world->IsEnemyInRange(selectedUnit, enemy)) // #TODO deze test buiten de methode uitvoeren?
    {
        PositionComponent* pc = dynamic_cast<PositionComponent*>(enemy->GetComponent(Component::POSITION));
        SelectTarget(pc->pos);

        Entity* ent = new Entity();
        world->Fight(selectedUnit, enemy, ent);
        engine->AddEntity(ent);
        success = true;
    }
    else
    {
        success = false;
    }

    return success;
}


bool AISystem::MoveUnitTowardsEntity(Entity* selectedUnit, Entity* entity)
{
    World* world = engine->GetContext()->GetWorld();
    Pathfinder* pathfinder = engine->GetContext()->GetPathfinder();

    vector<Grid> path = pathfinder->FindPath(selectedUnit, entity);

    if (path.size() > 0)
    {
        PositionComponent* upc = dynamic_cast<PositionComponent*>(selectedUnit->GetComponent(Component::POSITION));

        Grid lastStep = *(path.end() - 1);
        SelectTarget(Graphics::Instance().ToPx(lastStep));

        engine->GetContext()->SetGameState(Context::PL_MOVE);

        selectedUnit->Add(new AnimationComponent(path, upc->pos));
        engine->UpdateEntity(selectedUnit);

        return true;
    }
    else
    {
        return false;
    }
}


bool AISystem::MoveUnitTowardsEnemy(Entity* selectedUnit, Entity* enemy)
{
    World* world = engine->GetContext()->GetWorld();
    Pathfinder* pathfinder = engine->GetContext()->GetPathfinder();

    vector<Grid> path = pathfinder->FindPurePath(selectedUnit, enemy);

    if (path.size() > 0)
    {
        UnitComponent* uc = dynamic_cast<UnitComponent*>(selectedUnit->GetComponent(Component::UNIT));
        PositionComponent* upc = dynamic_cast<PositionComponent*>(selectedUnit->GetComponent(Component::POSITION));

        Grid lastStep = *(path.end() - 1);
        SelectTarget(Graphics::Instance().ToPx(lastStep));

        for (unsigned int i = 0; i <= uc->range_max; i++)
        {
            // last step is enemy unit so selectedUnit should not move there
            // unit should not move closer to enemy then is necessary to attack it
            path.pop_back();

            if (path.size() == 1)
            {
                // path should not disappear entirely
                break; 
            }
        }

        engine->GetContext()->SetGameState(Context::PL_MOVE);

        selectedUnit->Add(new AnimationComponent(path, upc->pos));
        engine->UpdateEntity(selectedUnit);

        return true;
    }
    else
    {
        return false;
    }
}


bool AISystem::MoveUnitAwayFromEnemyUntilOutsideMinRange(Entity* selectedUnit, Entity* enemy)
{
    World* world = engine->GetContext()->GetWorld();
    UnitComponent* auc = dynamic_cast<UnitComponent*>(selectedUnit->GetComponent(Component::UNIT));
    PositionComponent* apc = dynamic_cast<PositionComponent*>(selectedUnit->GetComponent(Component::POSITION));
    PositionComponent* dpc = dynamic_cast<PositionComponent*>(enemy->GetComponent(Component::POSITION));

    set<Entity*> moveOptions;
    unsigned int distanceToEnemy = world->CalcRange(apc, dpc);
    unsigned int minMoveDistance = auc->range_min - distanceToEnemy + 1;

    // make list of tiles around the enemy that the unit could move to
    for (unsigned int i = 0; i <= auc->range_max - distanceToEnemy; i++)
    {
        moveOptions = FindEntitiesAtDistanceAroundUnit(enemy, minMoveDistance + i);

        if (moveOptions.size() > 0)
        {
            break;
        }
    }

    // if walkable tiles were found
    if (moveOptions.size() > 0)
    {
        // find the one that's closest by
        Entity* moveOption = FilterNearestEntity(selectedUnit, moveOptions);

        if (moveOption != nullptr)
        {
            // and move the unit towards it
            return MoveUnitTowardsEntity(selectedUnit, moveOption);
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


void AISystem::SelectUnit(Entity* unit)
{
    unit->Add(new SelectionComponent());
    engine->UpdateEntity(unit);
}


void AISystem::SelectTarget(Vector2 pos)
{
    Entity* selectedTile = engine->GetEntityStream()->EntityWithTags({ Component::SELECTION, Component::HOVER });
    PositionComponent* tpc = dynamic_cast<PositionComponent*>(selectedTile->GetComponent(Component::POSITION));
    tpc->pos = pos;
    engine->UpdateEntity(selectedTile);
}


set<Entity*> AISystem::FindUnitsByPlayer(Entity::Player player)
{
    set<Entity*> units = engine->GetEntityStream()->WithTag(Component::UNIT);
    set<Entity*> playerUnits;

    copy_if(
        units.begin(),
        units.end(),
        inserter(playerUnits, playerUnits.end()),
        [&](Entity* unit)
    {
        UnitComponent* uc = dynamic_cast<UnitComponent*>(unit->GetComponent(Component::UNIT));
        return uc->player == player;
    }
    );

    return playerUnits;
}


set<Entity*> AISystem::FindEnemiesInsideRange(Entity* selectedUnit, unsigned int range)
{
    World* world = engine->GetContext()->GetWorld();
    PositionComponent* apc = dynamic_cast<PositionComponent*>(selectedUnit->GetComponent(Component::POSITION));
    PositionComponent* dpc;
    set<Entity*> enemies = FindUnitsByPlayer(Entity::HUMAN);
    set<Entity*> enemiesInsideRange;

    for (Entity* enemy : enemies)
    {
        dpc = dynamic_cast<PositionComponent*>(enemy->GetComponent(Component::POSITION));

        if (world->CalcRange(apc, dpc) <= range)
        {
            enemiesInsideRange.insert(enemy);
        }
    }

    return enemiesInsideRange;
}


set<Entity*> AISystem::FindEnemiesInsideMaxRange(Entity* selectedUnit)
{
    UnitComponent* auc = dynamic_cast<UnitComponent*>(selectedUnit->GetComponent(Component::UNIT));
    return FindEnemiesInsideRange(selectedUnit, auc->range_max);
}


set<Entity*> AISystem::FindEnemiesInsideMinRange(Entity* selectedUnit)
{
    UnitComponent* auc = dynamic_cast<UnitComponent*>(selectedUnit->GetComponent(Component::UNIT));
    return FindEnemiesInsideRange(selectedUnit, auc->range_min);
}


set<Entity*> AISystem::FilterEnemiesInRange(Entity* selectedUnit, set<Entity*> enemies)
{
    World* world = engine->GetContext()->GetWorld();
    set<Entity*> enemiesInRange;

    for (Entity* enemy : enemies)
    {
        if (world->IsEnemyInRange(selectedUnit, enemy))
        {
            enemiesInRange.insert(enemy);
        }
    }

    return enemiesInRange;
}


Entity* AISystem::FilterNearestUnit(Entity* selectedUnit, set<Entity*> units)
{
    Pathfinder* pathfinder = engine->GetContext()->GetPathfinder();

    Entity* nearest = nullptr;
    int pathSize;
    int shortestPathSize = INT_MAX;

    for (Entity* unit : units)
    {
        UnitComponent* uc = dynamic_cast<UnitComponent*>(unit->GetComponent(Component::UNIT));
        vector<Grid> path = pathfinder->FindPurePath(selectedUnit, unit);
        pathSize = static_cast<int>(path.size());

        if (pathSize > 0
            && (
                pathSize < shortestPathSize
                || (pathSize == shortestPathSize && uc->type == Entity::HQ) // if path sizes are equal, favor HQ
                )
            )
        {
            shortestPathSize = pathSize;
            nearest = unit;
        }
    }

    return nearest;
}


Entity* AISystem::FilterNearestEntity(Entity* selectedUnit, set<Entity*> entities)
{
    Pathfinder* pathfinder = engine->GetContext()->GetPathfinder();

    Entity* nearest = nullptr;
    int pathSize;
    int shortestPathSize = INT_MAX;

    for (Entity* entity : entities)
    {
        vector<Grid> path = pathfinder->FindPath(selectedUnit, entity);
        pathSize = static_cast<int>(path.size());

        if (pathSize > 0 && pathSize < shortestPathSize)
        {
            shortestPathSize = pathSize;
            nearest = entity;
        }
    }

    return nearest;
}


set<Entity*> AISystem::FindEntitiesAtDistanceAroundUnit(Entity* unit, unsigned int distance)
{
    World* world = engine->GetContext()->GetWorld();
    set<Entity*> entities;
    PositionComponent* pc = dynamic_cast<PositionComponent*>(unit->GetComponent(Component::POSITION));
    Grid pos = pc->pos.ToGrid();

    for (int i = -((int)distance); i <= (int)distance; i++)
    {
		Entity* ent;
		ent = world->getEntity(pos.row - distance, pos.col + i);
        if(ent)entities.insert(ent);
		ent = world->getEntity(pos.row + distance, pos.col + i);
        if(ent)entities.insert(ent);
		ent = world->getEntity(pos.row + i, pos.col - distance);
        if(ent) entities.insert(ent);
		ent = world->getEntity(pos.row + i, pos.col + distance);
        if(ent) entities.insert(ent);
    }

    return entities;
}
