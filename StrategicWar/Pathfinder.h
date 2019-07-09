#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include "Grid.h"
#include "Entity.h"
#include "World.h"
#include "DWGraph.h"

class Pathfinder {

public:
	Pathfinder(World* level, EntityStream*);

	~Pathfinder();

	std::vector<Grid> FindPath(Entity* start, Entity* end);
	std::vector<Grid> FindPurePath(Entity* start, Entity*end);

	int GetId(int row, int col, int width);

	int GetIdFromEntity(Entity* entity, int width);


	
private:
	
	EntityStream* es;
	DWGraph* graph;

	int width; //used to calculate id
	int height; //used for sizes of arrays

	int EstimateDistance(Entity& start, Entity& target);
	
	void AddUnits();

	void RemoveUnits();

	int GetDistance(Entity& start, Entity& end);

	void CreateGraph(World* level);

	std::vector<Grid> build_path(std::vector<Node*> came_from, Node* target, int distance);

	void ConnectNodeToNeighbors(Node* node);

	bool CanWalkOver(Entity& start, Node& end);
};







#endif
