#ifndef INFANTRY_BUILDER_H
#define INFANTRY_BUILDER_H

#include "EntityBuilder.h"

class InfantryBuilder : public EntityBuilder
{
public:
	InfantryBuilder(Entity::Player _player) : player(_player) {};
	virtual ~InfantryBuilder() {};
	virtual Entity* BuildEntity(int row, int col) {
		Entity* e = new Entity();
		e->InitAsUnit(Entity::INFANTRY, player, 10, 2, 2, 0, 1, row, col, 1, Graphics::SPRITE_INFANTRY);
		return e;
	}

private:
	Entity::Player player;
};

#endif
