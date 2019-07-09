#include "ScoreSystem.h"

#include <set>

using namespace std;

void ScoreSystem::SetEngine(Engine* engine) {
	this->engine = engine;

	//Create a new entity with a score component to keep track of the score during the game.
	if (engine != nullptr) {
		score = new Entity();
		score->Add(new ScoreComponent(0, 0, 0, 0));
		engine->AddEntity(score);
		updateData();
	}
}

void ScoreSystem::Update()
{
	//check for unit deaths
	for (Entity* ent : engine->GetEntityStream()->WithTag(Component::UNIT)) {
		if (dynamic_cast<UnitComponent*> (ent->GetComponent(Component::UNIT))->hp <= 0) {
			engine->RemoveEntity(ent);
		}
	}
	
	//The current gamestate
	Context::GameState gamestate = engine->GetContext()->GetGameState();

	//Update the scores and check if the AI has won.
	if (gamestate == Context::AI_FINISH) {
		//Only calculate the scores after a turn.
		updateData();
		ScoreComponent* sc = dynamic_cast<ScoreComponent*> (score->GetComponent(Component::SCORE));
		if (sc->playerUnits <= 0 || sc->totalPlayerHqHealth <= 0) {
			sc->aiWon = true;
		}
	}

	//Update the scores check if the player has won.
	else if (gamestate == Context::PL_FINISH || gamestate == Context::PL_PLAY) {
		updateData();
		ScoreComponent* sc = dynamic_cast<ScoreComponent*> (score->GetComponent(Component::SCORE));
		if (sc->aiUnits <= 0 || sc->totalAiHqHealth <= 0) {
			sc->playerWon = true;
		}
	}
	ScoreComponent* sc = dynamic_cast<ScoreComponent*> (score->GetComponent(Component::SCORE));
}

void ScoreSystem::updateData()
{

	int numberOfUnitsPlayer = 0;
	int numberOfUnitsAi = 0;
	int playerHqHealth = 0;
	int aiHqHealth = 0;

	//Recalculate the game.
	for (Entity* ent : engine->GetEntityStream()->WithTag(Component::UNIT)) {
		UnitComponent* uc = dynamic_cast<UnitComponent*>(ent->GetComponent(Component::UNIT));
		//Delete all dead units.
		if (uc->hp <= 0) {
			engine->RemoveEntity(ent);
		}
		//Count all alive units and update hqHealths.
		else {
			if (uc->player == Entity::Player::HUMAN) {
				if (uc->type == Entity::HQ) {
					playerHqHealth += uc->hp;
				}
				else {
					numberOfUnitsPlayer++;
				}
			}
			else if (uc->player == Entity::Player::AI) {
				if (uc->type == Entity::HQ) {
					aiHqHealth += uc->hp;
				}
				else {
					numberOfUnitsAi++;
				}
			}
		}
	}

	//Update the score entity.
	ScoreComponent* sc = dynamic_cast<ScoreComponent*> (score->GetComponent(Component::SCORE));
	sc->aiUnits = numberOfUnitsAi;
	sc->playerUnits = numberOfUnitsPlayer;
	sc->totalAiHqHealth = aiHqHealth;
	sc->totalPlayerHqHealth = playerHqHealth;
	engine->UpdateEntity(score);
}
