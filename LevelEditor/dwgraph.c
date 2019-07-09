#include "dwgraph.h"
#include "level.h"
#include "debug_file.h"

#include <stdlib.h>
#include <assert.h>


/*
    Adds new weighted edge to a Node to the neighbor. Does not check if the neighbor is in the same graph.
    If amount of neighbors goes over the maximum amount, the neighbor array will be doubled.
    (initial size: 4)
*/
static void add_neighbor_to_node(Node* node, Node* neighbor, int weight)
{
    //neighbor_array initialised
    if (node != NULL && neighbor != NULL)
    {
        if (node->max_neighbors == 0)
        {
            node->max_neighbors = 4;
            node->neighbor_array = (Node**)malloc(4 * sizeof(Node*));
            node->edges_weight_array = (int*)malloc(4 * sizeof(int));
        }
        //performed when max_neighbors is reached.
        //max_neighbors will usually be either 4 or 8.
        if ((++node->amount_of_neighbors) > node->max_neighbors)
        {
            node->max_neighbors *= 2;
            node->neighbor_array = realloc(node->neighbor_array, node->max_neighbors * sizeof(Node*));
            node->edges_weight_array = realloc(node->edges_weight_array, node->max_neighbors * sizeof(Node*));
        }
        node->neighbor_array[node->amount_of_neighbors - 1] = neighbor;
        node->edges_weight_array[node->amount_of_neighbors - 1] = weight;
    }
}

/*
    Calculates distance between 2 NEIGHBORING cells.
    CELLS MUST BE NEIGHBORS or it won't return the right value.
    Returns 0 if one of the cells is NULL or if cells are equal.
*/
static int get_distance_of_cells(Cell* start_cell, Cell* end_cell)
{
    if (start_cell != NULL && end_cell != NULL && start_cell != end_cell)
    {
        if (start_cell->row == end_cell->row || start_cell->col == end_cell->col)
        {
            if (start_cell->type == ROAD && end_cell->type == ROAD)
            {
                return ROAD_DISTANCE;
            }
            return HORIZONTAL_VERTICAL_DISTANCE;
        }
        else
        {
            return DIAGONAL_DISTANCE;
        }
    }
    else
    {
        return 0;
    }
}

/*
    Creates an edge from the starting node to the ending node with given weight.
    If weight == 0, method will calculate weight using the row and col value of the cells in start and end
*/
static void connect_nodes(DWGraph* graph, Node* start_node, Node* end_node, int weight) 
{
	if (start_node != end_node && graph != NULL && start_node != NULL && end_node != NULL) 
	{
		if (weight == 0)
		{
			//weight has to be calculated
			add_neighbor_to_node(start_node, end_node, get_distance_of_cells(start_node->cell, end_node->cell));
		}
		else
		{
			//use given weight
			add_neighbor_to_node(start_node, end_node, weight);
		}
	}
}

/*
    Method connects a node to his neighbors when necessary
    (depending on the cell type of given node & his neighbors in the level of the graph)
*/
static void connect_node_to_neighbors(DWGraph* graph, Node* node)
{
    if (graph != NULL && node != NULL)
    {
        // headquarter used for method level_can_walk_over
        Cell headquarter;
        headquarter.type = HEADQUARTER;

        int neighbor_x;
        int neighbor_y;

        int x = node->cell->col;
        int y = node->cell->row;

        Cell* neighbor_cell;

        if (level_can_walk_over(&headquarter, node->cell))
        {
            for (int delta_x = -1; delta_x <= 1; delta_x++)
            {
                for (int delta_y = -1; delta_y <= 1; delta_y++)
                {
                    neighbor_x = x + delta_x;
                    neighbor_y = y + delta_y;
                    neighbor_cell = &(graph->level->cells[neighbor_y][neighbor_x]);

                    if (level_is_valid_pos(graph->level, neighbor_y, neighbor_x) &&
                        level_can_walk_over(&headquarter,
                            neighbor_cell))
                    {
                        connect_nodes(graph, node,
                            get_node(graph, graph->level, neighbor_cell), 0);
                    }
                }
            }
        }
    }
}

/*
    Removes neighbor from a given node. The array will shift to
    put the empty spot at the end of the array.
    Only used in replace_node
*/
static void remove_neighbor_from_node(Node* node, Node* neighbor)
{
    if (node != NULL && neighbor != NULL)
    {
        int found = 0;
        for (int i = 0; i < node->amount_of_neighbors; i++)
        {
            if (found == 0)
            {
                if (node->neighbor_array[i] == neighbor)
                {
                    node->neighbor_array[i] = NULL;
                    node->edges_weight_array[i] = 0;
                    found = 1;
                    node->amount_of_neighbors--;
                }
            }
            else
            {
                node->neighbor_array[i - 1] = node->neighbor_array[i];
                node->edges_weight_array[i - 1] = node->edges_weight_array[i];
                node->neighbor_array[i] = NULL;
                node->edges_weight_array[i] = 0;
            }
        }
    }
}

/*
    Updates the weight of the edge between 2 nodes.
    If the edge doesn't exist, then it will be created using connect_nodes.
    Only used in replace_node
*/
static void update_neighbor_from_node(DWGraph* graph, Node* node, Node* neighbor, int weight)
{
    if (graph != NULL && node != NULL && neighbor != NULL)
    {
        int found = 0;
        for (int i = 0; i < node->amount_of_neighbors && found == 0; i++)
        {
            if (node->neighbor_array[i] == neighbor)
            {
                node->edges_weight_array[i] = weight;
                found = 1;
            }
        }
        if (found == 0)
        {
            connect_nodes(graph, node, neighbor, weight);
        }
    }
}

/*
    Method used to replace the celltype of a certain node in a graph. This will also replace the edges
    connected to this node (incoming and outgoing).
    Not used in this implementation. Could be used if the graph
    would be saved in a level & edited if the level changed.
*/
static void replace_node(DWGraph* graph, Node* node, Cell* cell)
{
    if (graph != NULL && node != NULL && cell != NULL)
    {
        if (cell->type != node->cell->type)
        {
            node->cell = cell;
            Cell headquarter;
            headquarter.type = HEADQUARTER;

            //assuming this is used for a level where every edge has a returning edge with the same weight

            if (!level_can_walk_over(&headquarter, cell))
            {
                for (int i = 0; i < node->amount_of_neighbors; i++)
                {
                    remove_neighbor_from_node(node->neighbor_array[i], node);
                }
                free(node->neighbor_array);
                node->amount_of_neighbors = 0;
                node->max_neighbors = 0;
            }
            else
            {
                free(node->neighbor_array);
                connect_node_to_neighbors(graph, node);
                for (int i = 0; i < node->amount_of_neighbors; i++)
                {
                    update_neighbor_from_node(graph, node->neighbor_array[i], node, get_distance_of_cells(node->neighbor_array[i]->cell, node->cell));
                }
            }

        }
        else
        {
            node->cell = cell;
        }
    }
}


DWGraph* create_empty_dwgraph(int amount_of_nodes) 
{
	int i = 0;
	DWGraph* graph = (DWGraph*)malloc(sizeof(DWGraph));
	Node* node_array = (Node*) calloc(amount_of_nodes,sizeof(Node));

	graph->amount_of_nodes = amount_of_nodes;
	graph->node_array = node_array;

	return graph;
}

DWGraph* create_dwgraph(Level* level) 
{
	int width = level->width;
	int height = level->height;

	DWGraph* graph =  create_empty_dwgraph(width * height);
	graph->level = level;
	if (level != NULL)
	{

		/*assign cells to nodes*/
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				Node* current_node = &(graph->node_array[y*width + x]);
				current_node->cell = &(level->cells[y][x]);
				current_node->max_neighbors = 0;
				current_node->amount_of_neighbors = 0;
			}
		}

		{
			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					connect_node_to_neighbors(graph, get_node(graph, level, &level->cells[y][x]));
				}
			}
		}
	}
		return graph;
}

void free_dwgraph(DWGraph* graph) 
{
	if (graph != NULL) {
		Node* node = graph->node_array;
		for (int i = 0; i < graph->amount_of_nodes; i++)
		{
			if (node[i].max_neighbors != 0)
			{
				free(node[i].neighbor_array);
				free(node[i].edges_weight_array);
				node[i].amount_of_neighbors = 0;
				node[i].max_neighbors = 0;
			}

		}

		free(graph->node_array);
		graph->amount_of_nodes = 0;
		free(graph);
	}
}

Node* get_node(DWGraph* graph, Level* level, Cell* cell)
{
    /*place of cell in array is calculated. Only works with graphs built with the create_dwgraph(Level*)*/
    Node*  node = &(graph->node_array[(cell->row)*(level->width) + (cell->col)]);
    return node;
}

int get_distance(Node* start, Node* end) 
{
	if (start != NULL && end != NULL)
	{
		for (int i = 0; i < start->amount_of_neighbors; i++)
		{
			if (start->neighbor_array[i] == end)
			{
				return start->edges_weight_array[i];
			}
		}
	}
	return 0;
}