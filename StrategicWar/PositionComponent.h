#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include "Component.h"
#include "Graphics.h"

class PositionComponent : public Component
{
public:
	Vector2 pos;
	//z-axis to know to know rendering order of overlapping Entities
	int z;

	PositionComponent(Grid _pos, int _z) : pos(Graphics::Instance().ToPx(_pos)), z(_z) {};
	PositionComponent(Vector2 _pos) : pos(_pos) {};
	PositionComponent(Vector2 _pos, int _z) : pos(_pos), z(_z) {};

	virtual ~PositionComponent() {};
	virtual Tag GetTag() { return POSITION; };
};

#endif
