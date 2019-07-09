#ifndef VECTOR2_H
#define VECTOR2_H

#include "constants.h"
#include "Grid.h"


class Vector2
{
public:
	float x, y;

	Vector2() : x(0), y(0) {};
	Vector2(float _x, float _y) : x(_x), y(_y) {};

	~Vector2() {};

    Grid ToGrid() { return Grid((int)y / GRID_SIZE, (int)x / GRID_SIZE); };

	//Operators
	bool operator ==(const Vector2& vector2)
	{
		return vector2.y == y && vector2.x == x;
	}

	bool operator !=(const Vector2& vector2)
	{
		return vector2.y != y || vector2.x != x;
	}

	bool operator <(const Vector2& vector2)
	{
		return y < vector2.y && x < vector2.x;
	}

	bool operator >(const Vector2& vector2)
	{
		return y > vector2.y && x > vector2.x;
	}
};

#endif