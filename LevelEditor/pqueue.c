#include "pqueue.h"
#include "debug_file.h"

#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>


static int smallest_child(PriorityQueue* q, int index)
{
    int result_index = 2 * index;
    if (result_index < q->item_count && q->items[result_index + 1].cost < q->items[result_index].cost)
    {
        result_index++;
    }
    return result_index;
}

static void percolate_down(PriorityQueue* q, int index)
{
    QueueItem item_to_replace = q->items[index];
    if (q->item_count >= 2 * index)
    {
        int child_index = smallest_child(q, index);
        int successful = 1;
        while (successful && q->items[child_index].cost < item_to_replace.cost)
        {
            q->items[index] = q->items[child_index];
            index = child_index;
            if (2 * index <= q->item_count)
            {
                child_index = smallest_child(q, index);
            }
            else
            {
                successful = 0;
            }

        }
        q->items[index] = item_to_replace;
    }
}

static void percolate_up(PriorityQueue* q, int index)
{
    if (index == 1) return;
    QueueItem item_to_replace = q->items[index];
    int parent_index = index / 2;
    int successful = 1;
    while (successful && q->items[parent_index].cost > item_to_replace.cost)
    {
        q->items[index] = q->items[parent_index];
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
    q->items[index] = item_to_replace;
}


PriorityQueue* pqueue_alloc()
{
	int initial_max_items = 10;
	PriorityQueue* res = (PriorityQueue*)calloc(1, sizeof(PriorityQueue));
	res->item_max_count = initial_max_items;
	res->item_count = 0;
	res->items = (QueueItem*)calloc(initial_max_items, sizeof(QueueItem));

	assert(res->items[0].item == NULL);
	assert(res->items[0].cost == 0);
	assert(res->items[initial_max_items - 1].item == NULL);
	assert(res->items[initial_max_items - 1].cost == 0);

	return res;
}

void pqueue_free(PriorityQueue* queue)
{
	queue->item_max_count = 0;
	queue->item_count = 0;
	free(queue->items);
	free(queue);
}

void pqueue_update(PriorityQueue* q, void* item, int cost)
{
	assert(q != NULL);
	assert(item != NULL);
	for (int i = 1; i <= q->item_count; i++)
	{
		if (q->items[i].item == item)
		{
			if (q->items[i].cost < cost)
			{
				q->items[i].cost = cost;
				percolate_down(q, i);
			}
			else 
			{
				q->items[i].cost = cost;
				percolate_up(q, i);
			}
			return;
		}
	}
	if (q->item_count >= q->item_max_count - 1)
	{
		int old_size = q->item_max_count;
		int new_size = q->item_max_count * 2;
		q->items = realloc(q->items, new_size * sizeof(QueueItem));
		q->item_max_count = new_size;
	}
	int free_index = ++q->item_count;

	QueueItem item_to_store;
	item_to_store.item = item;
	item_to_store.cost = cost;
	q->items[free_index] = item_to_store;
	
	percolate_up(q, free_index);
}

void* pqueue_remove_first(PriorityQueue* q)
{
	assert(q != NULL);
	assert(q->item_count != 0);
	QueueItem item_to_remove = q->items[1];
	QueueItem last_item = q->items[q->item_count];
	q->items[1] = last_item;
	q->item_count--;
	percolate_down(q, 1);
	return item_to_remove.item;
}
