#include "UnitBuilder.h"

using namespace std;

UnitBuilder::UnitBuilder()
{
	/*playerSpriteMap[Entity::AI] = Graphics::SPRITE_FLAG2;
	playerSpriteMap[Entity::HUMAN] = Graphics::SPRITE_FLAG1;*/
	flag1 = new TextureComponent(Graphics::SPRITE_FLAG1);
	flag2 = new TextureComponent(Graphics::SPRITE_FLAG2);
	unitSpriteMap[Entity::ARCHERY] = Graphics::SPRITE_ARCHERY;
	unitSpriteMap[Entity::INFANTRY] = Graphics::SPRITE_INFANTRY;
	unitSpriteMap[Entity::FIRE] = Graphics::SPRITE_FIRE;
	unitSpriteMap[Entity::HQ] = Graphics::SPRITE_HQ;
}

UnitBuilder::~UnitBuilder(){}
vector<Entity*> UnitBuilder::BuildEntities(int row, int col, Entity::Player player, Entity::UnitType type)
{
	vector<Entity*> result;
	Entity* unit = new Entity();
	Entity* flag = new Entity();
	unit->InitAsUnit(type, player, 10, 3, 3, 0, 3, row, col, 1, unitSpriteMap.find(type)->second);
	result.push_back(unit);
	PositionComponent* pos = (PositionComponent*)unit->GetComponent(Component::POSITION);
	if (player == Entity::HUMAN)
		flag->Add(flag1);
	else if (player == Entity::AI)
		flag->Add(flag2);
	flag->Add(pos);
	result.push_back(flag);
	return result;
}