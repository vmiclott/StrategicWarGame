#include "GamestateSystem.h"
#include <chrono>
#include <thread>

using namespace std;


void GameStateSystem::Update()
{
    Context::GameState gameState = engine->GetContext()->GetGameState();

    // during play stage of either player
    if (gameState == Context::PL_PLAY || gameState == Context::AI_PLAY)
    {
        PauseUnitsWithoutAP();
    }
    // if the player has finished, make it the AI's turn
    else if (gameState == Context::PL_FINISH)
    {
        EndTurn(Entity::HUMAN);
        engine->GetContext()->SetGameState(Context::AI_PLAY);
    }
    // if the AI has finished, make it the player's turn
    else if (gameState == Context::AI_FINISH) 
    {
        // don't end abruptly: wait a short while
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        EndTurn(Entity::AI);
        engine->GetContext()->SetGameState(Context::PL_PLAY);
    }
}


void GameStateSystem::EndTurn(Entity::Player player)
{
    DeselectCurrentUnit();
    ResetUnitsAP();
    ClearPausedUnits();
}


void GameStateSystem::ClearPausedUnits()
{
    EntityStream* es = engine->GetEntityStream();
    set<Entity*> pausedUnits = es->WithTag(Component::PAUSE);

    for (Entity* unit : pausedUnits)
    {
        UnpauseUnit(unit);
    }
}


void GameStateSystem::DeselectCurrentUnit()
{
    Entity* selectedUnit = engine->GetEntityStream()->EntityWithTags({ Component::SELECTION, Component::UNIT });

    if (selectedUnit != nullptr)
    {
        SelectionComponent* sc = dynamic_cast<SelectionComponent*>(selectedUnit->GetComponent(Component::SELECTION));
        sc->state = SelectionComponent::TO_DELETE;
    }
}


void GameStateSystem::ResetUnitsAP()
{
    for (Entity* unit : engine->GetEntityStream()->WithTag(Component::UNIT))
    {
        UnitComponent* uc = (UnitComponent*)unit->GetComponent(Component::UNIT);
        uc->ap = uc->maxAp;
    }
}


void GameStateSystem::PauseUnitsWithoutAP()
{
	boolean continuePlayerTurn = false;
	boolean continueAITurn = false;
    for (Entity* unit : engine->GetEntityStream()->WithTag(Component::UNIT))
    {
        if (unit->GetComponent(Component::PAUSE) == NULL) // don't pause the same unit twice
        {
			
            UnitComponent* uc = (UnitComponent*)unit->GetComponent(Component::UNIT);

			if (uc->type != Entity::HQ) {
				if (uc->player == Entity::HUMAN) {
					continuePlayerTurn = true;
				}
				else {
					continueAITurn = true;
				}
			}


            if (uc->ap == 0)
            {
                PauseUnit(unit);
            }
        }
    }

	if (engine->GetContext()->GetGameState() == Context::PL_PLAY && !continuePlayerTurn) {
		engine->GetContext()->SetGameState(Context::PL_FINISH);
	}
	else if (engine->GetContext()->GetGameState() == Context::AI_PLAY && !continueAITurn){
		engine->GetContext()->SetGameState(Context::AI_FINISH);
	}


}


void GameStateSystem::PauseUnit(Entity* unit) 
{
    SelectionComponent* sc = dynamic_cast<SelectionComponent*>(unit->GetComponent(Component::SELECTION));

    // if the unit is selected, deselect it
    if (sc != nullptr)
    {
        sc->state = SelectionComponent::TO_DELETE;
    }

    // pause the unit
    unit->Add(new PauseComponent);
    AddPausedTexture(unit);
    engine->UpdateEntity(unit);
}


void GameStateSystem::UnpauseUnit(Entity* unit)
{
    RemovePausedTexture(unit);

    PauseComponent* pc = dynamic_cast<PauseComponent*>(unit->GetComponent(Component::PAUSE));
    delete (unit->Remove(pc));
    engine->UpdateEntity(unit);
}


void GameStateSystem::AddPausedTexture(Entity* entity)
{
    Graphics::Sprite sprite = Graphics::SPRITE_WAIT;
    TextureComponent* tc = dynamic_cast<TextureComponent*>(entity->GetComponent(Component::TEXTURE));

    if (tc) {
        tc->textures.push_back(sprite);
    }
    else {
        tc = new TextureComponent({ sprite });
        entity->Add(tc);
    }
}


void GameStateSystem::RemovePausedTexture(Entity* entity)
{
    Graphics::Sprite sprite = Graphics::SPRITE_WAIT;
    TextureComponent* tc = dynamic_cast<TextureComponent*>(entity->GetComponent(Component::TEXTURE));

    if (tc)
    {
        vector<Graphics::Sprite>* textures = &tc->textures;
        vector<Graphics::Sprite>::iterator it = textures->begin();

        while (*it != sprite && it < textures->end() - 1)
        {
            it++;
        }

        if (*it == sprite)
        {
            textures->erase(it);
        }
    }
}