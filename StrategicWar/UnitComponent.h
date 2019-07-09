#ifndef UNIT_COMPONENT_H
#define UNIT_COMPONENT_H

#include "Component.h"
#include "Entity.h"

class UnitComponent : public Component
{
public:
	
	Entity::UnitType type;
	Entity::Player player;
	int hp;
	int ap;
	int maxAp;
	int dp;
	unsigned int range_min;
	unsigned int range_max;

	UnitComponent(Entity::UnitType _type, Entity::Player _player, int _hp, int _ap, int _dp, unsigned int _range_min, unsigned int _range_max) :
		type(_type), player(_player), hp(_hp), ap(_ap), maxAp(_ap), dp(_dp), range_min(_range_min), range_max(_range_max) {};

	virtual ~UnitComponent() {};
	virtual Tag GetTag() { return UNIT; };
};


#endif
