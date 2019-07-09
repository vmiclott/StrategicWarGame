#ifndef WORLD_H
#define WORLD_H

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include "Entity.h"
#include "Engine.h"
#include "constants.h"
#include "ArcherBuilder.h"
#include "BridgeBuilder.h"
#include "EntityBuilder.h"
#include "FireBuilder.h"
#include "GroundBuilder.h"
#include "HqBuilder.h"
#include "InfantryBuilder.h"
#include "RockBuilder.h"
#include "WaterBuilder.h"

typedef std::vector<std::vector<Entity*>> tileMatrix;

typedef std::map<char, EntityBuilder*> builderMap;

class World
{
public:
	World();
	~World();

	//initializes the world based on the given filename.
	void Init(std::string world_file, Engine* engine);
	int GetWidth();
	int GetHeight();

	//Returns a matrix holding the tiles.
	tileMatrix GetMatrix();

	//Returns the Entity in the matrix at the given coordinates
	Entity* getEntity(int row, int col);

	//Returns all the possible tiles in this world;
	std::vector<Entity*> GetGroundTiles();

	//Returns the currently selected unit.
    //Entity* GetCurrentUnit() { return currentUnit; };

	//Sets the current unit to play with.
    //void SetCurrentUnit(Entity* unit) { currentUnit = unit; };

	//Handles the fighting of 2 units.
	void Fight(Entity* attacker, Entity* defender, Entity* attackAnimation);

	//Calculates the range between 2 squares.
	unsigned int CalcRange(PositionComponent* attacker, PositionComponent* defender);

    bool IsEnemyInRange(Entity* attacker, Entity* defender);


private:
	//Fields
    int width;
	int height;

	//A 2-dimensional vector to hold the different tiles.
	tileMatrix matrix;

	//A map that has characters as key and builders of entities as value.
	builderMap builders;

	//A vector to hold all the ground tiles. 
	std::vector<Entity*> groundTiles;

	//Methods
	void SetWidth(int w);
	void SetHeight(int h);
	void SetDimensions(std::string width, std::string height);
	void EntityFromCharacter(char c, int row, int col, Engine* engine);
	void ClearWorld();
	void DamageUnit(Entity*, int);

	//vector holding the sprites for health
	std::vector<Graphics::Sprite> health;

};

#endif