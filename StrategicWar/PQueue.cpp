#include "PQueue.h"
#include "debug_file.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>

using namespace std;

template<class T>
QueueItem<T>::QueueItem(T* item, int cost)
{
	this->item = item;
	this->cost = cost;
}

template<class T>
QueueItem<T>::~QueueItem() {}

template<class T>
T* QueueItem<T>::GetItem() 
{
	return item;
}

template<class T>
int QueueItem<T>::GetCost()
{
	return cost;
}

template<class T>
void QueueItem<T>::SetCost(int cost)
{
	this->cost = cost;
}


template<class T>
int PQueue<T>::SmallestChild(int index)
{
	int result_index = 2 * index;
	if (result_index < items.size() - 1 && items[result_index + 1].GetCost() < items[result_index].GetCost())
	{
		result_index++;
	}
	return result_index;
};

template<class T>
void PQueue<T>::PercolateDown(int index)
{
	QueueItem<T> item_to_replace = items[index];
	if (items.size() > 2 * index)
	{
		int child_index = SmallestChild(index);
		int successful = 1;
		while (successful && items[child_index].GetCost() < item_to_replace.GetCost())
		{
			items[index] = items[child_index];
			index = child_index;
			if (2 * index < items.size())
			{
				child_index = SmallestChild(index);
			}
			else
			{
				successful = 0;
			}

		}
		items[index] = item_to_replace;
	}
}

template<class T>
void PQueue<T>::PercolateUp(int index)
{
	if (index == 1) return;
	QueueItem<T> item_to_replace = items[index];
	int parent_index = index / 2;
	int successful = 1;
	while (successful && items[parent_index].GetCost() > item_to_replace.GetCost())
	{
		items[index] = items[parent_index];
		index = parent_index;
		if (index / 2 > 0)
		{
			parent_index = index / 2;
		}
		else
		{
			successful = 0;
		}

	}
	items[index] = item_to_replace;

}

template<class T>
PQueue<T>::PQueue()
{
	
}

template<class T>
PQueue<T>::~PQueue()
{
	delete[] items;
}

template<class T>
void PQueue<T>::Update(T* item, int cost)
{
	assert(item != NULL);
	for (int i = 1; i < items.size(); i++)
	{
		if (items[i].GetItem() == item)
		{
			if (items[i].GetCost() < cost)
			{
				items[i].SetCost(cost);
				PercolateDown(i);
			}
			else
			{
				items[i].SetCost(cost);
				PercolateUp(i);
			}
			return;
		}
	}

	QueueItem<T> item_to_store = new QueueItem(item, cost);

	items.push_back(item_to_store);

	PercolateUp(items.size()-1);
}

template<class T>
T* PQueue<T>::Pop()
{
	assert(items.size() != 0);
	QueueItem<T> item_to_remove = items[1];
	QueueItem<T> last_item = items[items.size()-1];
	items[1] = last_item;
	items.erase(items.rend());
	PercolateDown(1);
	return item_to_remove.GetItem();
}

template<class T>
int PQueue<T>::Size() {
	return items.size()-1;
}