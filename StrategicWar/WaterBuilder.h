#ifndef WATER_BUILDER_H
#define WATER_BUILDER_H

#include "EntityBuilder.h"

class WaterBuilder : public EntityBuilder
{
public:
	WaterBuilder() {};
	virtual ~WaterBuilder() {};
	virtual Entity* BuildEntity(int row, int col) {
		Entity* e = new Entity();
		e->InitAsTile(row, col, 1, GroundtypeComponent::WATER, Graphics::SPRITE_WATER);
		return e;
	}
};

#endif