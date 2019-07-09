#ifndef FIRE_BUILDER_H
#define FIRE_BUILDER_H

#include "EntityBuilder.h"

class FireBuilder : public EntityBuilder
{
public:
	FireBuilder(Entity::Player _player) : player(_player) {};
	virtual ~FireBuilder() {};
	virtual Entity* BuildEntity(int row, int col) {
		Entity* e = new Entity();
		e->InitAsUnit(Entity::FIRE, player, 10, 1, 4, 2, 5, row, col, 1, Graphics::SPRITE_FIRE);
		return e;
	}

private:
	Entity::Player player;
};

#endif
