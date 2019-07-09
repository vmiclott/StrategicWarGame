#include "pathfinder.h"
#include "debug_file.h"

#include "dwgraph.h"
#include "pqueue.h"
#include "level.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>


static int estimate_distance(Node* start, Node* target) {
	int col_difference = abs(start->cell->col - target->cell->col);
	int row_difference = abs(start->cell->row - target->cell->row);
	int diagonal_steps = min(col_difference,row_difference);
	int non_diagonal_steps = max(col_difference, row_difference) - diagonal_steps;
	
	//if roads are present:
	return diagonal_steps*2*ROAD_DISTANCE+non_diagonal_steps*ROAD_DISTANCE;
	

	//if roads aren't present:
	// return diagonal_steps * DIAGONAL_DISTANCE + non_diagonal_steps * HORIZONTAL_VERTICAL_DISTANCE;
}

/*
	given the came_from-array, a path is built by backtracking
*/
static Path* build_path(DWGraph* graph, Level* level, Node** came_from, Node* target, int distance)
{
	int width = level->width;

	Node* original_target = target;

	Node* from = came_from[get_id(width, target->cell)];

	int count = 1;

	while (from != NULL) {
		count++;
		int id = get_id(width, from->cell);
		target = from;
		from = came_from[id];
	}

	Path* path = path_alloc(count, distance);
	from = original_target;

	//path built backwards
	for (int i = count-1; i >= 0; i--) {
		path->steps[i] = pos_init(from->cell->row, from->cell->col);
		from = came_from[get_id(width, from->cell)];
	}
	free_dwgraph(graph);
	free(came_from);
	return path;
}

/*
    function to give each cell a unique id to save in arrays ('hashfunction' for cells)
*/
static int get_id(int width, Cell* cell)
{
	return cell->row*width + cell->col;
}

/*
	algorithm used: A*-algorithm with estimate_distance as function.
*/
Path* find_path(Level* level, Cell* start, Cell* target)
{
	//check if start/target is valid
	if (start == NULL || target == NULL 
     || !level_is_valid_pos(level, start->row, start->col) || !level_is_valid_pos(level, target->row, target->col))
	{
		return NULL;
	}
	//check if start/target are on top of eachother (shouldn't happen)
	if (start == target)
	{
		Path* path = path_alloc(1,0);
		path->steps[0] = pos_init(start->row, start->col);
		return path;
	}

	int width = level->width;
	int height = level->height;

	//graph is built
	DWGraph* graph = create_dwgraph(level);
	//target_node saved for estimate_distance(..., target_node)
	Node* target_node = get_node(graph, level, target);
	//an array with values to indicate if the shortest distance was discovered to a node, using an id for cells
	int* already_visited_array = calloc(width*height, sizeof(int));

	PriorityQueue* queue = pqueue_alloc();
	//array to keep track of which node was visited last before this node to reach the shortest distance to this node
	Node** came_from = calloc(width*height, sizeof(Node*));
	//array to keep track of the distance from start to current_node
	int* real_distance = calloc(width*height, sizeof(int));
	
	pqueue_update(queue, get_node(graph, level, start), 0);

	came_from[get_id(width, start)] = NULL;

	int first_iteration = 1;
	Node* current_node;
	int id;
	int neighbor_id;
	while (queue->item_count > 0)
	{
		//takes the node with shortest distance to start & estimated distance to target
		current_node = pqueue_remove_first(queue);
		id = get_id(width, current_node->cell);
		already_visited_array[id] = 1;


		//in first iteration, came_from[id] is NULL
		if (!first_iteration) 
		{
			real_distance[id] = real_distance[get_id(width, came_from[id]->cell)] + get_distance(came_from[id], current_node);
		}
		
		if (current_node->cell == target)
		{
			//shortest route to target found: initialize path creation in seperate method
			int total_distance = real_distance[id];
			//already free arrays/queue not necessary for path creation
			free(already_visited_array);
			free(real_distance);
			pqueue_free(queue);
			return build_path(graph, level, came_from, current_node, total_distance);
		}
		//go over neighbors of current node
		for (int i = 0; i < current_node->amount_of_neighbors; i++)
		{
			Node* neighbor = current_node->neighbor_array[i];
			neighbor_id = get_id(width, neighbor->cell);
			//if neighbor hasn't been 'visited' yet: hasn't been the 'current_node' in a loop
			if (already_visited_array[neighbor_id] == 0)
			{
				int distance_neighbor = real_distance[id] + current_node->edges_weight_array[i];
				//checks if the current distance to the neighbor is better than a previous distance
				if (real_distance[neighbor_id] == 0 || distance_neighbor < real_distance[neighbor_id])
				{
					pqueue_update(queue, neighbor, distance_neighbor + estimate_distance(neighbor, target_node));
					real_distance[neighbor_id] = distance_neighbor;
					came_from[neighbor_id] = current_node;
				}
			}
		}
		first_iteration = 0;
	}
	//no path was found
	free(already_visited_array);
	free(real_distance);
	free_dwgraph(graph);
	free(came_from);
	pqueue_free(queue);
	return NULL;

}

//int min(int a, int b) {
//	if (a < b) {
//		return a;
//	}
//	return b;
//}
//
//int max(int a, int b) {
//	if (a > b) {
//		return a;
//	}
//	return b;
//}




