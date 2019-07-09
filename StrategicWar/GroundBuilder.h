#ifndef GROUND_BUILDER_H
#define GROUND_BUILDER_H

#include "EntityBuilder.h"

class GroundBuilder : public EntityBuilder
{
public:
	GroundBuilder() {};
	virtual ~GroundBuilder() {};
	virtual Entity* BuildEntity(int row, int col) {
		Entity* e = new Entity();
		e->InitAsTile(row, col, 0, GroundtypeComponent::GROUND, Graphics::SPRITE_TERRAIN);
		return e;
	}
};

#endif
