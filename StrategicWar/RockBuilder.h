#ifndef ROCK_BUILDER_H
#define ROCK_BUILDER_H

#include "EntityBuilder.h"

class RockBuilder : public EntityBuilder
{
public:
	RockBuilder() {};
	virtual ~RockBuilder() {};
	virtual Entity* BuildEntity(int row, int col) {
		Entity* e = new Entity();
		e->InitAsTile(row, col, 1, GroundtypeComponent::ROCK, Graphics::SPRITE_ROCK);
		return e;
	}
};

#endif
