#include "EntityStream.h"
#include "Component.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;

EntityStream::EntityStream() {}

EntityStream::~EntityStream() 
{
	
}

set<Entity*> EntityStream::WithTag(Component::Tag tag)
{
	return index[tag];
}

set<Entity*> EntityStream::WithTags(set<Component::Tag> tags)
{
	set<Component::Tag>::iterator iter = tags.begin();
	set<Entity*> toReturn = index[*iter];

	set<Entity*> currentSet;
	set<Component::Tag>::iterator end = tags.end();

	iter++;

	while (iter != end) {
		currentSet = index[*iter];
		Entity* entity;
		set<Entity*>::iterator toReturnIter = toReturn.begin();
		while(toReturnIter != toReturn.end()) {
			entity = *toReturnIter;
			toReturnIter++;
			if (currentSet.find(entity) == currentSet.end()) {
				toReturn.erase(entity);
			}
		}
		iter++;
	}

	return toReturn;
}

Entity* EntityStream::EntityWithTags(set<Component::Tag> tags)
{
    set<Entity*> entities = WithTags(tags);

    if (entities.size() == 1)
    {
        return *entities.begin();
    }
    else
    {
        return nullptr;
    }
}

/* Call this whenever an Entity is added to the Engine */
void EntityStream::EntityAdded(Entity* entity)
{
	// For each tag of entity
	vector<Component::Tag> tags = entity->GetTags();
	for (Component::Tag iter: tags)
	{
		AddEntityWithTag(entity, iter);
	}
}

/* Call this whenever an Entity that was already added to the Engine, changes its components */
void EntityStream::EntityChanged(Entity* entity) {
	EntityRemoved(entity);
	EntityAdded(entity);
}

void EntityStream::AddEntityWithTag(Entity* entity, Component::Tag tag)
{
	index[tag].insert(entity);
}

/* Call this whenever an Entity is removed from the Engine */
void EntityStream::EntityRemoved(Entity* entity)
{
    // werkt niet wanneer je een component van een entity verwijdert en daarna de entity update
	/*for (Component::Tag tag : entity->GetTags()) {
		RemoveEntityWithTag(entity, tag);
	}*/

    // For each existing tag
    map<Component::Tag, set<Entity*>>::iterator it = index.begin();
    while (it != index.end()) {
        RemoveEntityWithTag(entity, it->first);
        ++it;
    }
}

void EntityStream::RemoveEntityWithTag(Entity* entity, Component::Tag tag)
{
	index[tag].erase(entity);
}