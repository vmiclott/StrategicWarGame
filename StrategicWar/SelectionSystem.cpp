#include "SelectionSystem.h"

using namespace std;


void SelectionSystem::Update()
{
    Entity* selectedUnit = FindSingleSelectedEntity(Component::UNIT);
    Entity* selectedTile = FindSingleSelectedEntity(Component::HOVER);

    // if selected units were found, draw the selection frames
    for (Entity* selectedEntity : { selectedUnit, selectedTile })
    {
        if (selectedEntity != nullptr)
        {
            SelectionComponent* sc = dynamic_cast<SelectionComponent*>(selectedEntity->GetComponent(Component::SELECTION));

            if (sc->state == SelectionComponent::NEW)
            {
                AddSelectionTexture(selectedEntity);
                engine->UpdateEntity(selectedEntity);

                sc->state = SelectionComponent::OLD;
            }
        }
    }

    // if there is both a selected unit and a selected tile, draw the path between them
    if (selectedUnit != NULL && selectedTile != NULL)
    {
        if (selectedUnit->GetComponent(Component::ANIMATION) != NULL)
        {
            // don't display the path while an animation is playing
            ClearCurrentPath();
        }
        else
        {
            Grid selectedPos = dynamic_cast<PositionComponent*>(selectedTile->GetComponent(Component::POSITION))->pos.ToGrid();

            if (selectedPos != lastHoveredPos)
            {
                CreateCurrentPath(selectedUnit, selectedTile);
                lastHoveredPos = selectedPos;
                AddSelectionTexture(selectedTile);
            }
        }
    }
    else
    {
        ClearCurrentPath();

        if (selectedTile != NULL)
        {
            AddSelectionTexture(selectedTile);
        }
    }
}

void SelectionSystem::SetEngine(Engine * engine)
{
	
	this->engine = engine;
	if (engine) {
		lastHoveredEntity = new Entity();
		lastHoveredEntity->Add(new PositionComponent(Grid(-1,-1), 3));
		lastHoveredEntity->Add(new TextureComponent());
		
		engine->AddEntity(lastHoveredEntity);
	}
}


Entity* SelectionSystem::FindSingleSelectedEntity(Component::Tag tag)
{
    EntityStream* es = engine->GetEntityStream();
    set<Entity*> selectedEntities = es->WithTags({ Component::SELECTION, tag });
    Entity* selectedEntity = nullptr;

    // if there is more than one, take the new one and deselect the others
    for (Entity* entity : selectedEntities)
    {
        SelectionComponent* sc = dynamic_cast<SelectionComponent*>(entity->GetComponent(Component::SELECTION));

        if (selectedEntity == nullptr && sc->state == SelectionComponent::NEW)
        {
            selectedEntity = entity;
        }
        else if (selectedEntities.size() == 1 && sc->state != SelectionComponent::TO_DELETE)
        {
            selectedEntity = entity;
        }
        else
        {
            RemoveSelectionTexture(entity);
            delete (entity->Remove(sc));
            engine->UpdateEntity(entity);
        }
    }

    return selectedEntity;
}


void SelectionSystem::AddSelectionTexture(Entity* entity)
{
    Graphics::Sprite sprite = GetSelectionSpriteForEntity(entity);
    TextureComponent* tc = dynamic_cast<TextureComponent*>(entity->GetComponent(Component::TEXTURE));

    RemoveSelectionTexture(entity); // in case same unit is selected twice.

    if (tc) {
        tc->textures.push_back(sprite);
    }
    else {
        tc = new TextureComponent({ sprite });
        entity->Add(tc);
    }
}


void SelectionSystem::RemoveSelectionTexture(Entity* entity)
{
    TextureComponent* tc = dynamic_cast<TextureComponent*>(entity->GetComponent(Component::TEXTURE));

    if (tc)
    {
		tc->removeTexture(Graphics::SPRITE_SELECT_HIGHLIGHT);
        tc->removeTexture(Graphics::SPRITE_SELECT);
        tc->removeTexture(Graphics::SPRITE_CROSS_GREY);
        tc->removeTexture(Graphics::SPRITE_CROSSHAIR);
		tc->removeTexture(Graphics::SPRITE_DESTINATION);
    }
}


Graphics::Sprite SelectionSystem::GetSelectionSpriteForEntity(Entity* entity)
{
    // entity is unit
    if (entity->GetComponent(Component::UNIT) != NULL)
    {
        return Graphics::SPRITE_SELECT_HIGHLIGHT;
    }
    // entity is hover texture
    else
    {
        Entity* selectedUnit = engine->GetEntityStream()->EntityWithTags({ Component::SELECTION, Component::UNIT });

        // if there is no currently selected unit
        if (selectedUnit == nullptr)
        {
            return Graphics::SPRITE_SELECT;
        }
        // if there is a selected unit
        else
        {
            Context::GameState gameState = engine->GetContext()->GetGameState();
            World* world = engine->GetContext()->GetWorld();
            Entity* unitOnTile = FindUnitOnTile(entity);

            // if there is a unit on the hovered tile
            if (unitOnTile != nullptr)
            {
                UnitComponent* uc = dynamic_cast<UnitComponent*>(unitOnTile->GetComponent(Component::UNIT));

                // if the unit is an enemy
                if ((gameState == Context::PL_PLAY && uc->player == Entity::AI)
                    || (gameState == Context::AI_PLAY && uc->player == Entity::HUMAN)
                    )
                {
                    // if the unit is within range
                    if (world->IsEnemyInRange(selectedUnit, unitOnTile))
                    {
                        return Graphics::SPRITE_CROSSHAIR;
                    }
                    else
                    {
                        return Graphics::SPRITE_SELECT;
                    }
                }
                // if the unit is an ally
                else
                {
                    return Graphics::SPRITE_SELECT;
                }
            }
            // if the hovered tile is terrain
            else
            {
                // if the tile is unreachable for the selectedunit
                if (currentPath.size() == 0)
                {
                    return Graphics::SPRITE_CROSS_GREY;
                }
                // if it is reachable
                else
                {
					UnitComponent* uc = dynamic_cast<UnitComponent*>(selectedUnit->GetComponent(Component::UNIT));

					if (uc && static_cast<int>(currentPath.size()) <= uc->ap) 
                    {
                        // no feet underneath SPRITE_DESTINATION
						engine->RemoveEntity(*currentPath.rbegin());
						currentPath.pop_back();
						return Graphics::SPRITE_DESTINATION;
					}
					else {
						return Graphics::SPRITE_SELECT;
					}
                }
            }
        }
    }
}


void SelectionSystem::CreateCurrentPath(Entity* unit, Entity* tile)
{
    World* world = engine->GetContext()->GetWorld();
    Pathfinder* pathfinder = engine->GetContext()->GetPathfinder();

    int remainingAP = dynamic_cast<UnitComponent*>(unit->GetComponent(Component::UNIT))->ap;
    std::vector<Grid> path = pathfinder->FindPath(unit, tile);

    ClearCurrentPath();

	// make new Entities with the selection sprite and add them to the engine
	int counter = 1;

	for (unsigned int i = 0; i < path.size(); i++)
	{
		Entity* step = new Entity();

		if (counter <= remainingAP)
		{
			
			step->Add(new TextureComponent({ Graphics::SPRITE_PATH }));
			
		}
		else
		{
			step->Add(new TextureComponent({ Graphics::SPRITE_PATH_FAR }));
		}

		step->Add(new PositionComponent(path[i], 1));
		engine->AddEntity(step);
		currentPath.push_back(step);

		counter++;
	}
}


void SelectionSystem::ClearCurrentPath()
{
	if ( ! currentPath.empty()) 
    {
        for (std::vector<Entity*>::size_type i = 0; i != currentPath.size(); i++) 
        {
            engine->RemoveEntity(currentPath[i]);
        }

        currentPath.clear();
	}
}


Entity* SelectionSystem::FindUnitOnTile(Entity* tile)
{
    PositionComponent* pc = dynamic_cast<PositionComponent*>(tile->GetComponent(Component::POSITION));
    Entity* unitOnTile = nullptr;

    for (Entity* i : engine->GetEntityStream()->WithTags({ Component::UNIT })) 
    {
        if (dynamic_cast<PositionComponent*>(i->GetComponent(Component::POSITION))->pos == pc->pos) 
        {
            unitOnTile = i;
            break;
        }
    }

    return unitOnTile;
}
