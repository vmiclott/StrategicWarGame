#include "RenderSystem.h"
#include "Component.h"
#include "UnitComponent.h"
#include "TextComponent.h"
#include "ScoreComponent.h"
#include "World.h"

#include <iostream>

using namespace std;

void RenderSystem::Update() 
{
	map<int, std::vector<Entity*>> toDraw;
	map<int, std::vector<Entity*>> stringToDraw;

	int maxZ = 0;

	if (AllegroLib::Instance().IsRedraw()) {
		Graphics::Instance().ClearScreen();

		Graphics::Instance().DrawBitmap(Graphics::SPRITE_HUD, 0, 0);
		Graphics::Instance().DrawBitmap(Graphics::SPRITE_WORLD, 0, 0);

		for (Entity* i : engine->GetEntityStream()->WithTag(Component::TEXTURE)) {
			if (i->GetComponent(Component::TEXT) == nullptr)
			{
				if (i->GetComponent(Component::GROUNDTYPE) == nullptr) {
					PositionComponent* pos = (PositionComponent*)i->GetComponent(Component::POSITION);
					if (pos->z > maxZ) maxZ = pos->z;
					toDraw[pos->z].push_back(i);
				}
			}
			else
			{
				vector<Graphics::Sprite> textures = ((TextureComponent*)i->GetComponent(Component::TEXTURE))->textures;
				PositionComponent* pos = (PositionComponent*)i->GetComponent(Component::POSITION);
				if (pos->z > maxZ) maxZ = pos->z;
				toDraw[pos->z].push_back(i);
				stringToDraw[pos->z].push_back(i);
			}
		}

		for (int i = 0; i < maxZ; i++) {
			for (Entity* ent: toDraw[i]) {
				for (Graphics::Sprite sprite : ((TextureComponent*)ent->GetComponent(Component::TEXTURE))->textures) {
					Vector2 vector = dynamic_cast<PositionComponent*>(ent->GetComponent(Component::POSITION))->pos;
					Graphics::Instance().DrawBitmap(sprite, vector.x, vector.y);
				}
			}
			for (Entity* text : stringToDraw[i]) {
				string toPrint = dynamic_cast<TextComponent*>(text->GetComponent(Component::TEXT))->text;
				Vector2 vector = dynamic_cast<PositionComponent*>(text->GetComponent(Component::POSITION))->pos;
				Graphics::Instance().DrawString(toPrint, vector.x + 15, vector.y, Color(13, 26, 129), Graphics::ALIGN_CENTER, false);
			}
		}

		//Find out the game status to render to the infosquares.
		//Calculate the selected unit's stats.
		Entity* currentUnit = engine->GetEntityStream()->EntityWithTags({ Component::SELECTION, Component::UNIT });
		string unitHp = "";
		string unitAp = "";
		string unitDp = "";
		string unitMin = "";
		string unitMax = "";
		if (currentUnit != NULL) {
			UnitComponent* uc = dynamic_cast<UnitComponent*>(currentUnit->GetComponent(Component::UNIT));
			std::stringstream sstm;
			sstm << unitHp << uc->hp;
			std::stringstream sstm2;
			sstm2 << unitAp << uc->ap;
			std::stringstream sstm3;
			sstm3 << unitDp << uc->dp;
			std::stringstream sstm4;
			sstm4 << unitMin << uc->range_min;
			std::stringstream sstm5;
			sstm5 << unitMax << uc->range_max;
			unitHp = sstm.str();
			unitAp = sstm2.str();
			unitDp = sstm3.str();
			unitMin = sstm4.str();
			unitMax = sstm5.str();
		}
		
		//Calculate the player's HQ health.
		string hqHealth = "HQ: ";
		Entity* score;
		for (Entity* ent : engine->GetEntityStream()->WithTag(Component::SCORE)) {
			score = ent;
		}
		if (score != NULL) {
			ScoreComponent* sc = dynamic_cast<ScoreComponent*>(score->GetComponent(Component::SCORE));
			std::stringstream sstm;
			sstm << hqHealth << sc->totalPlayerHqHealth;
			hqHealth = sstm.str();
		}

		//InfoSquares.
		//Current unit hp.
		//Graphics::Instance().DrawRectangle(10, 440, 150, 35, Color(13, 26, 129), 1);
		Graphics::Instance().DrawString(unitHp, 90, 390, Color(13, 26, 129), Graphics::ALIGN_LEFT, false);
		//Current unit ap.
		//Graphics::Instance().DrawRectangle(10, 480, 150, 35, Color(13, 26, 129), 1);
		Graphics::Instance().DrawString(unitAp, 90, 420, Color(13, 26, 129), Graphics::ALIGN_LEFT, false);

		//Current unit damage.
		Graphics::Instance().DrawString(unitDp, 90, 450, Color(13, 26, 129), Graphics::ALIGN_LEFT, false);
		//Current unit rangemin.
		Graphics::Instance().DrawString(unitMin, 90, 480, Color(13, 26, 129), Graphics::ALIGN_LEFT, false);
		//Current unit rangemax.
		Graphics::Instance().DrawString(unitMax, 90, 510, Color(13, 26, 129), Graphics::ALIGN_LEFT, false);

		//Current unit maxRange
		//Players total hq health.
		Graphics::Instance().DrawRectangle(10, 600, 150, 35, Color(13, 26, 129), 1);
		Graphics::Instance().DrawString(hqHealth, 15, 565, Color(13, 26, 129), Graphics::ALIGN_LEFT, false);

		Graphics::Instance().ExecuteDraws();
	}
}