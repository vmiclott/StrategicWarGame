#ifndef PQUEUE_H
#define PQUEUE_H


#include <vector>

using namespace std;

/*
* This struct represents an item in a PriorityQueue
*/


template<class T>
class QueueItem
{
public:
	QueueItem(T* item, int cost);
	~QueueItem();

	T* GetItem();
	int GetCost();
	void SetCost(int cost);

private:
	T* item;
	int cost;
};


template<class T>
class PQueue
{
public:
	PQueue();
	~PQueue();

	/*
	* Updates the cost associated with a given item in the provided priority_queue.
	*
	* If the item is not yet in the PriorityQueue, it is added.
	*/
	void Update(T* item, int cost);

	/*
	* Searches the item with the lowest cost,
	* removes it from the PriorityQueue,
	* and returns the item.
	*/
	T* Pop();

	int Size();

private:

	vector<QueueItem<T>> items;
	
	int SmallestChild(int index);
	void PercolateUp(int index);
	void PercolateDown(int index);


};




#endif

