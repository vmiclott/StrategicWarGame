#ifndef UNIT_BUILDER_H
#define UNIT_BUILDER_H
#include <map>
#include <vector>
#include "Entity.h"
#include "Component.h"
#include "PositionComponent.h"
#include "TextureComponent.h"
#include "GroundtypeComponent.h"
#include "constants.h"

class UnitBuilder
{
public:
	UnitBuilder();
	virtual ~UnitBuilder();
	virtual std::vector<Entity*> BuildEntities(int row, int col, Entity::Player player, Entity::UnitType type);


private:
	TextureComponent* flag1;
	TextureComponent* flag2;
	std::map<Entity::UnitType, Graphics::Sprite> unitSpriteMap;
};

#endif
