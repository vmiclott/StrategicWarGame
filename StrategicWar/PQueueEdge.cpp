#include "PQueueEdge.h"
#include "debug_file.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>

using namespace std;

QueueItemEdge::QueueItemEdge(Edge* item, int cost)
{
	this->item = item;
	this->cost = cost;
}

QueueItemEdge::~QueueItemEdge() {}

Edge* QueueItemEdge::GetItem()
{
	return item;
}

int QueueItemEdge::GetCost()
{
	return cost;
}

void QueueItemEdge::SetCost(int cost)
{
	this->cost = cost;
}


int PQueueEdge::SmallestChild(int index)
{
	int result_index = 2 * index;
	if (result_index < static_cast<int>(items.size()) - 1 
     && items[result_index + 1].GetCost() < items[result_index].GetCost())
	{
		result_index++;
	}
	return result_index;
};

void PQueueEdge::PercolateDown(int index)
{
	QueueItemEdge item_to_replace = items[index];
	if (item_count >= 2 * index)
	{
		int child_index = SmallestChild(index);
		bool successful = 1;
		while (successful && items[child_index].GetCost() < item_to_replace.GetCost())
		{
			items[index] = items[child_index];
			index = child_index;
			if (2 * index <= item_count)
			{
				child_index = SmallestChild(index);
			}
			else
			{
				successful = false;
			}

		}
		items[index] = item_to_replace;
	}
}

void PQueueEdge::PercolateUp(int index)
{
	if (index == 1) return;
	QueueItemEdge item_to_replace = items[index];
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

PQueueEdge::PQueueEdge()
{
	items.push_back(QueueItemEdge(nullptr, 0));

	item_count = 0;
}

PQueueEdge::~PQueueEdge()
{
	items.clear();
}

void PQueueEdge::Update(Edge* item, int cost)
{
	assert(item != NULL);
	for (int i = 1; i <= item_count; i++)
	{
		if (*items[i].GetItem() == *item)
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

	QueueItemEdge item_to_store = QueueItemEdge(item, cost);

	if (item_count+1 >= static_cast<int>(items.size())) {
		items.push_back(item_to_store);
	}
	else {
		items[item_count+1] = item_to_store;
	}

	item_count++;

	PercolateUp(item_count);
}

Edge* PQueueEdge::Pop()
{
	assert(items.size() != 0);
	QueueItemEdge item_to_remove = items[1];
	QueueItemEdge last_item = items[item_count];
	items[1] = last_item;
	item_count--;
	PercolateDown(1);
	return item_to_remove.GetItem();
}

int PQueueEdge::Size()
{
	return item_count;
}