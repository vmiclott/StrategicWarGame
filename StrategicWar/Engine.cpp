#include "Engine.h"
#include "System.h"
#include "EntityStream.h"
#include "AllegroLib.h"
#include "Tags.h"

#include <algorithm>
#include <map>

using namespace std;

Engine::~Engine() 
{
	for (Entity* ent : entities) {
		delete ent;
	}
}

void Engine::AddEntity(Entity* entity)
{
	//Add entity to engine
	entities.push_back(entity);
	//Warn EntityStream
	EntityAdded(entity);
}

void Engine::UpdateEntity(Entity* entity)
{
	//Warn EntityStream
	EntityChanged(entity);
}

void Engine::RemoveEntity(Entity* entity)
{
	//Remove entity from engine
	entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
	//Warn EntityStream
	EntityRemoved(entity);

	delete(entity);
}

void Engine::AddSystem(System* system)
{
    // Add System to engine
    systems.push_back(system);
    //Set engine pointer of system to this engine
    system->SetEngine(this);
}

void Engine::RemoveSystem(System* system)
{
	//Remove System from engine
	systems.erase(std::remove(systems.begin(), systems.end(), system), systems.end());
	//Reset engine pointer of system to NULL
	system->SetEngine(NULL);

}

vector<Entity*> Engine::GetEntities()
{
	vector<Entity*> result;
	
	//Make a set of all tags.
	std::set<Component::Tag> tags = { Component::TEXTURE, Component::UNIT, Component::POSITION, Component::ANIMATION, Component::GROUNDTYPE };
	
	for (std::set<Component::Tag>::iterator it = tags.begin(); it != tags.end(); ++it) {
		set<Entity*> temp = es.WithTag(*it);
		for (std::set<Entity*>::iterator it2 = temp.begin(); it2 != temp.end(); ++it2) {
			result.push_back(*it2);
		}
	}
	return result;
}

void Engine::Update()
{
	//Call Update() on all assigned systems in a fixed order.
	for (std::vector<System*>::size_type i = 0; i != systems.size(); i++) {
		systems[i]->Update();
	}
}


void Engine::EntityAdded(Entity* entity)
{
	es.EntityAdded(entity);
}

void Engine::EntityChanged(Entity* entity)
{
	es.EntityChanged(entity);
}

void Engine::EntityRemoved(Entity* entity)
{
	es.EntityRemoved(entity);
}

Context* Engine::GetContext()
{
	//Return context pointer
	return context;
}

System* Engine::GetSystem(System::Type type)
{
	//Return requested System
	std::map<System::Type, System*>::iterator it = index.find(type);
	if (it != index.end())
		return it->second;
	return NULL;
}

EntityStream* Engine::GetEntityStream()
{
	//Return EntityStream
	return &es;
}