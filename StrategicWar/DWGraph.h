#ifndef DWGRAPH_H
#define DWGRAPH_H

#include "EntityStream.h"

class Edge;


class Node
{
public:
	Node();
	Node(Entity*);
	~Node();
	std::vector<Edge*> GetEdges();
	void AddEdge(Node* end, int weight);
	Entity* GetEntity();
	void SetEntity(Entity*);

	Entity::UnitType* GetType();
	void SetType(Entity::UnitType*);


private:
	std::vector<Edge*> edges;
	Entity* entity;

	Entity::UnitType* type;
};


class Edge
{
public:
	Edge(Node* start, Node* end, int weight);
	~Edge();
	Node* GetStart();
	Node* GetEnd();
	int GetWeight();

	/*
	2 edges are equal if there target nodes are equal.
	this is to ensure correctness of pathfinding with the priorityqueue.
	*/
	bool operator==(const Edge& edge2);

	/*
	checks for all parameters if it's equal with edge2, since operator == is overloaded for the algorithm.
	*/
	bool Equals(const Edge& edge2);

private:
	Node* start;
	Node* end;
	int weight;
};

class DWGraph
{
public:
	DWGraph(int size);
	~DWGraph();
	Node* GetNode(int id);
	void addEntity(Entity*, int id);
	void ConnectNodes(Node& start, Node& end, int weight);

private:
	Node* nodes;
	int size;
};

#endif
