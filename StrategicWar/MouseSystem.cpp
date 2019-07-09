#include "MouseSystem.h"
#include "World.h"
#include <set>
#include <algorithm>

using namespace std;

MouseSystem::MouseSystem()
{
    //Initialize selectedTile
    selectedTile = new Entity();
    selectedTile->Add(new SelectionComponent);
    selectedTile->Add(new HoverComponent);
    selectedTile->Add(new PositionComponent(Grid(0, 0), 2));

    //Initialize allegro
    al = &AllegroLib::Instance();
}

void MouseSystem::SetEngine(Engine* engine)
{
    if (engine)
    {
        this->engine = engine;
        engine->AddEntity(selectedTile);
    }
}

void MouseSystem::Update()
{
    World* world = engine->GetContext()->GetWorld();

    //Grids made as boundaries for the level
    Grid upperLeft(0, 0);
    Grid bottomRight(world->GetHeight(), world->GetWidth());

    if (engine->GetContext()->GetGameState() == Context::PL_PLAY)
    {
        //Hover event
        if (al->IsMouseEvent()) {

            ALLEGRO_EVENT ev = al->GetCurrentEvent();
            Vector2 vector2(ev.mouse.x, ev.mouse.y);
            Grid grid = Graphics::Instance().FromPx(vector2);

            if (grid < bottomRight && grid >= upperLeft) {

                //Update the old position of the selectionTile.
                Entity* selectedTile = engine->GetEntityStream()->EntityWithTags({ Component::SELECTION, Component::HOVER });
                PositionComponent* pc = dynamic_cast<PositionComponent*>(selectedTile->GetComponent(Component::POSITION));
                pc->pos = Graphics::Instance().ToPx(grid);
                engine->UpdateEntity(selectedTile);
            }
        }

        //Click Event
        else if (al->IsMouseClickEvent())
        {
            // left click
            if (al->GetCurrentEvent().mouse.button == 1) {

                ALLEGRO_EVENT ev = al->GetCurrentEvent();
                Vector2 vector2(ev.mouse.x, ev.mouse.y);
                Grid grid = Graphics::Instance().FromPx(vector2);

                if (grid < bottomRight && grid >= upperLeft)
                {
                    //Select a tile by getting it from the matrix in world and checking the entitystream to know if there's a unit on the tile.
                    Entity* tile = world->getEntity(grid.row, grid.col);
                    std::set<Entity*> entities = engine->GetEntityStream()->WithTag(Component::UNIT);
                    for (Entity* ent : entities) {
                        PositionComponent* position = dynamic_cast<PositionComponent*>(ent->GetComponent(Component::POSITION));
                        if (grid == Graphics::Instance().FromPx(position->pos)) {
                            tile = ent;
                        }
                    }

                    //There was a unit selected.
                    Entity* currentUnit = engine->GetEntityStream()->EntityWithTags({ Component::SELECTION, Component::UNIT });
                    if (currentUnit != NULL) {

                        //Attack
                        if (tile->GetComponent(Component::UNIT) != NULL &&
                            ((UnitComponent*)tile->GetComponent(Component::UNIT))->player == Entity::Player::AI) {
                            Attack(currentUnit, vector2, tile);
                        }
                        else if (tile->GetComponent(Component::UNIT) != NULL &&
                            ((UnitComponent*)tile->GetComponent(Component::UNIT))->player == Entity::Player::HUMAN &&
                            ((UnitComponent*)tile->GetComponent(Component::UNIT))->type != Entity::UnitType::HQ &&
                            ((UnitComponent*)tile->GetComponent(Component::UNIT))->ap>0) {
                            SelectUnit(tile);
                        }
                        //Move the unit.
                        else if (tile->GetComponent(Component::UNIT) == NULL) {
                            //Calculate path to selected tile
                            std::vector<Grid> path = engine->GetContext()->GetPathfinder()->FindPath(currentUnit, tile);
                            Move(currentUnit, vector2, path);
                        }
                    }

                    //There was no unit selected.
                    else
                    {
                        //Select the unit if its a unit. The unit should not be a HQ.
                        if (tile->GetComponent(Component::UNIT) != NULL
                            && ((UnitComponent*)tile->GetComponent(Component::UNIT))->player == Entity::Player::HUMAN
                            && ((UnitComponent*)tile->GetComponent(Component::UNIT))->type != Entity::UnitType::HQ
                            && ((UnitComponent*)tile->GetComponent(Component::UNIT))->ap > 0
                            )
                        {
                            SelectUnit(tile);
                        }
                    }
                }
            }
            // right click
            else if (al->GetCurrentEvent().mouse.button == 2)
            {
                DeselectCurrentUnit();
            }
        }
    }
}

void MouseSystem::SelectUnit(Entity* unit)
{
    unit->Add(new SelectionComponent());
    engine->UpdateEntity(unit);
}

void MouseSystem::DeselectCurrentUnit()
{
    Entity* selectedUnit = engine->GetEntityStream()->EntityWithTags({ Component::SELECTION, Component::UNIT });

    if (selectedUnit != nullptr)
    {
        SelectionComponent* sc = dynamic_cast<SelectionComponent*>(selectedUnit->GetComponent(Component::SELECTION));
        sc->state = SelectionComponent::TO_DELETE;
    }
}

void MouseSystem::Attack(Entity* unit, Vector2 destination, Entity* AIUnit)
{
    //Calculate if the units are within attacking range.
    PositionComponent* attackerPos = dynamic_cast<PositionComponent*>(unit->GetComponent(Component::POSITION));
    PositionComponent* defenderPos = dynamic_cast<PositionComponent*>(AIUnit->GetComponent(Component::POSITION));
    unsigned int range = engine->GetContext()->GetWorld()->CalcRange(attackerPos, defenderPos);

    //Get the unitComponents and start fighting when they are within range.
    UnitComponent* PlayerUc = dynamic_cast<UnitComponent*>(unit->GetComponent(Component::UNIT));
    UnitComponent* AiUc = dynamic_cast<UnitComponent*>(AIUnit->GetComponent(Component::UNIT));
    if (range >= PlayerUc->range_min && range <= PlayerUc->range_max) {
        engine->GetContext()->SetGameState(Context::PL_ATTACK);
        Entity* ent = new Entity();
        engine->GetContext()->GetWorld()->Fight(unit, AIUnit, ent);
        engine->AddEntity(ent);
    }
}

void MouseSystem::Move(Entity* unit, Vector2 destination, std::vector<Grid> path)
{
    //check if path is empty
    if (path.empty()) return;
    //path isn't empty: move the unit

    //Set the gamestate
    engine->GetContext()->SetGameState(Context::PL_MOVE);

    UnitComponent* uc = dynamic_cast<UnitComponent*>(unit->GetComponent(Component::UNIT));

    //animationsystem takes care of actual moving.
    unit->Add(new AnimationComponent(path, ((PositionComponent*)unit->GetComponent(Component::POSITION))->pos));
    engine->UpdateEntity(unit);
}
