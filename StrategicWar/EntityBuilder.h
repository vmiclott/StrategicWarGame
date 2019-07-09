#ifndef ENTITY_BUILDER_H
#define ENTITY_BUILDER_H

#include "Entity.h"

using namespace std;

class Graphics;

class EntityBuilder
{
public:
	virtual ~EntityBuilder() {};
	virtual Entity* BuildEntity(int row, int col) = 0;
};

#endif