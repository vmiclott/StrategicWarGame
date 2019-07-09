#ifndef ARCHER_BUILDER_H
#define ARCHER_BUILDER_H

#include "EntityBuilder.h"

class ArcherBuilder : public EntityBuilder 
{
public:
	ArcherBuilder(Entity::Player _player) : player(_player) {};
	virtual ~ArcherBuilder() {};
	virtual Entity* BuildEntity(int row, int col) {
		Entity* e = new Entity();
		e->InitAsUnit(Entity::ARCHERY, player, 10, 3, 3, 0, 3, row, col, 1, Graphics::SPRITE_ARCHERY);
		return e;
	}

private:
	Entity::Player player;
};

#endif
