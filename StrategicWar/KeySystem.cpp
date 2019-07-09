#include "KeySystem.h"
#include "World.h"
#include "SelectionComponent.h"
#include "PauseComponent.h"
#include "UnitComponent.h"

KeySystem::KeySystem()
{
	//Initialize allegro
	al = &AllegroLib::Instance();
}

void KeySystem::Update()
{
	// Mark changes in keymap
	if (engine->GetContext()->GetGameState() == Context::PL_PLAY && al->IsKeyboardEvent()) 
    {
		ALLEGRO_EVENT ev = al->GetCurrentEvent();
		// TODO: Register which keys were pressed

        // ESCAPE: deselect currently selected unit
		if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) 
        {
            Entity* selectedUnit = engine->GetEntityStream()->EntityWithTags({ Component::SELECTION, Component::UNIT });

            if (selectedUnit != nullptr)
            {
                SelectionComponent* sc = dynamic_cast<SelectionComponent*>(selectedUnit->GetComponent(Component::SELECTION));
                sc->state = SelectionComponent::TO_DELETE;
            }
		}

        // ENTER: end current turn
		if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) 
        {
            Context::GameState gameState = engine->GetContext()->GetGameState();

            if (gameState == Context::PL_PLAY)
            {
                engine->GetContext()->SetGameState(Context::PL_FINISH);
            }
            else if(gameState == Context::AI_PLAY)
            {
				//for testing purposes
                //engine->GetContext()->SetGameState(Context::AI_FINISH);
            }
		}

        // SPACE: pause currently selected unit
		if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) 
        {
			for (Entity* selected : engine->GetEntityStream()->WithTag(Component::SELECTION)) 
            {
				if (selected) 
                {
					UnitComponent* uc = dynamic_cast<UnitComponent*>(selected->GetComponent(Component::UNIT));
					if (uc) uc->ap = 0;
				}
			}
		}
	}
}
