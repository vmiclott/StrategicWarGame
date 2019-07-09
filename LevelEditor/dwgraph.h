#ifndef DWGRAPH_H
#define DWGRAPH_H

#include "level.h"
#include "common.h"

typedef struct Node Node;


struct Node
{
	int amount_of_neighbors;
	int max_neighbors;

	Node** neighbor_array;
	int* edges_weight_array;

	Cell* cell;

};

typedef struct
{
	int amount_of_nodes;
	Node* node_array;
	Level* level;

} DWGraph;


/*
    Creates a directed weighted graph with given amount of nodes without edges.
*/
DWGraph* create_empty_dwgraph(int amount_of_nodes);

/*
    Creates a directed weighted graph fitting the given level.
	Creates an empty graph if level == NULL.
*/
DWGraph* create_dwgraph(Level* level);

/*
    Frees memory of graph
*/
void free_dwgraph(DWGraph* graph);

/*
    Returns node belonging to given Cell & Level in graph
*/
Node* get_node(DWGraph* graph, Level* level, Cell* cell);

/*
	Returns weight of directed edge between given nodes. 
	Returns 0 if the edge doesn't exist
*/
int get_distance(Node* start, Node* end);

#endif
