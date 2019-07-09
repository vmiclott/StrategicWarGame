#ifndef HQ_BUILDER_H
#define HQ_BUILDER_H

#include "EntityBuilder.h"

class HqBuilder : public EntityBuilder
{
public:
	HqBuilder(Entity::Player _player) : player(_player) {};
	virtual ~HqBuilder() {};
	virtual Entity* BuildEntity(int row, int col) {
		Entity* e = new Entity();
		e->InitAsUnit(Entity::HQ, player, 10, 3, 3, 0, 3, row, col, 1, Graphics::SPRITE_HQ);
		return e;
	}

private:
	Entity::Player player;
};

#endif
