#ifndef GROUNDTYPE_COMPONENT_H
#define GROUNDTYPE_COMPONENT_H

#include "Component.h"

class GroundtypeComponent : public Component
{
public:

	enum GroundType {
		GROUND, ROCK, WATER, ROAD, BRIDGE
	};

	GroundType type;

	GroundtypeComponent(GroundType type) { this->type = type; }

	virtual ~GroundtypeComponent() {};
	virtual Tag GetTag() { return GROUNDTYPE; };
};

#endif