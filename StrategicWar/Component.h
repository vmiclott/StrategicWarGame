#ifndef COMPONENT_H
#define COMPONENT_H

#include "Vector2.h"
#include "Grid.h"

class Component
{
public:
	enum Tag {
        ANIMATION,
        HOVER, 
        GROUNDTYPE,
        PAUSE, 
        POSITION, 
        SCORE,
        SELECTION, 
        TEXTURE,
		TEXT,
        UNIT,
        NUMBER_OF_COMPONENTS
	};

	Component() {};
	virtual ~Component() {};
	virtual Tag GetTag() = 0;

};

#endif