#ifndef PQUEUEEDGE_H
#define PQUEUEEDGE_H


#include <vector>
#include "Entity.h"
#include "DWGraph.h"

using namespace std;

/*
* This struct represents an item in a PriorityQueue
*/


class QueueItemEdge
{
public:
	QueueItemEdge(Edge* item, int cost);
	~QueueItemEdge();

	Edge* GetItem();
	int GetCost();
	void SetCost(int cost);

private:
	Edge* item;
	int cost;
};


class PQueueEdge
{
public:
	PQueueEdge();
	~PQueueEdge();

	/*
	* Updates the cost associated with a given item in the provided priority_queue.
	*
	* If the item is not yet in the PriorityQueue, it is added.
	*/
	void Update(Edge* item, int cost);

	/*
	* Searches the item with the lowest cost,
	* removes it from the PriorityQueue,
	* and returns the item.
	*/
	Edge* Pop();

	int Size();

private:

	vector<QueueItemEdge> items;
	int item_count;

	int SmallestChild(int index);
	void PercolateUp(int index);
	void PercolateDown(int index);
	


};




#endif

