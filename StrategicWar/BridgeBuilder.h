#ifndef BRIDGE_BUILDER_H
#define BRIDGE_BUILDER_H

#include "EntityBuilder.h"

class BridgeBuilder : public EntityBuilder
{
public:
	BridgeBuilder() {};
	virtual ~BridgeBuilder() {};
	virtual Entity* BuildEntity(int row, int col) {
		Entity* e = new Entity();
		e->InitAsTile(row, col, 1, GroundtypeComponent::BRIDGE, Graphics::SPRITE_BRIDGE);
		return e;
	}
};

#endif
