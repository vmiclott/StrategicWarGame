#include "Entity.h"

#include <map>
#include <vector>
#include <iostream>
#include "Graphics.h"
#include "UnitComponent.h"

using namespace std;

Entity::Entity()
{
	components;
}

Entity::~Entity()
{
	DeleteAllComponents();
}

void Entity::InitAsUnit(UnitType type, Player player, int hp, int ap, int dp, unsigned int range_min, unsigned int range_max, int row, int col, int z_axis, Graphics::Sprite sprite)
{
	//First make sure that the components map is empty.
	components.clear();
	Grid gr = Grid(row, col);
	UnitComponent* uc = new UnitComponent(type, player, hp, ap, dp, range_min, range_max);
	PositionComponent* pc = new PositionComponent(gr, z_axis);
	TextureComponent* tc1;
	if (player == Entity::HUMAN)
		tc1 = new TextureComponent({ sprite, Graphics::SPRITE_FLAG1 });
	else if (player == Entity::AI)
		tc1 = new TextureComponent({ sprite, Graphics::SPRITE_FLAG2 });
	Add(uc);
	Add(pc);
	Add(tc1);
}

void Entity::InitAsTile(int row, int col, int z_axis, GroundtypeComponent::GroundType type, Graphics::Sprite sprite)
{
	//First make sure that the components map is empty.
	components.clear();
	Grid gr = Grid(row, col);
	PositionComponent* pc = new PositionComponent(gr, z_axis);
	TextureComponent* tc = new TextureComponent({ sprite });
	GroundtypeComponent* gc = new GroundtypeComponent(type);
	Add(pc);
	Add(tc);
	Add(gc);
}

void Entity::InitAsFlag(PositionComponent* pc, Entity::Player player)
{
	components.clear();
	TextureComponent* tc;
	if (player == Entity::HUMAN)
		tc = new TextureComponent({ Graphics::SPRITE_FLAG1 });
	else if (player == Entity::AI)
		tc = new TextureComponent({ Graphics::SPRITE_FLAG2 });
	Add(pc);
	Add(tc);
}

void Entity::DeleteAllComponents()
{
	for (Component* c : GetComponents())
	{
		if (&c != NULL)
			delete c;
	}
}

Entity* Entity::Add(Component * component) 
{
	//Add this component to the set stored under the Component's key. If the component already exists, no value is added.
    if (components[component->GetTag()] == NULL)
    {
        components[component->GetTag()] = component;
    }
    else
    {
        delete component;
    }
	
	//Return a pointer to this Entity to allow a builder pattern.
	return this;
}

Component* Entity::GetComponent(Component::Tag tag)
{
	//Return the component associated with the given tag. Null if the Entity does not contain the given Component.
	componentmap::const_iterator it = components.find(tag);
	if (it != components.end())
		return it->second;
	return NULL;
}

Component* Entity::Remove(Component * component)
{
	if (component == nullptr) return nullptr;
	Component* tempComponent = component;
	//Remove this component entry form the componentmap. If the component is not in the map, nothing happens.
	components.erase(component->GetTag());
	//Return a pointer to this component.
	return tempComponent;
}

vector<Component::Tag> Entity::GetTags()
{
	//Return a vector with all assigned tags 
	std::vector<Component::Tag> componentsKeys;
	for (pair<Component::Tag,Component*> imap : components)
		componentsKeys.push_back(imap.first);
	return componentsKeys;
}

vector<Component*> Entity::GetComponents()
{
	//Return a vector with all assigend components. 
	std::vector<Component*> componentsValues;
	for (componentmap::const_iterator it = components.begin(); it != components.end(); ++it) {
		componentsValues.push_back(it->second);
	}
	return componentsValues;
}