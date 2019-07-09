#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <vector>

#include "Component.h"
#include "PositionComponent.h"
#include "TextureComponent.h"
#include "GroundtypeComponent.h"
#include "constants.h"

class Graphics;
class UnitComponent;

typedef std::map<Component::Tag, Component*> componentmap;

class Entity
{
public:
	Entity();
	~Entity();

	enum UnitType {
		ARCHERY,
		INFANTRY,
		FIRE,
		HQ,
		NUMBER_OF_UNIT_TYPES
	};

	enum Player {
		AI,
		HUMAN,
		OWNER_NONE,
		NUMBER_OF_PLAYERS
	};

	/*Initialize the Entity as a unit.*/
	void InitAsUnit(UnitType ut, Player pl, int hp, int ap, int dp, unsigned int range_min, unsigned int range_max,
					int row, int col, int z_axis,
					Graphics::Sprite sprite);

	/*Initialize the Entity as tile.*/
	void InitAsTile(int row, int col, int z_axis, GroundtypeComponent::GroundType type,
		Graphics::Sprite sprite);

	/*Initialize the Entity as flag.*/
	void InitAsFlag(PositionComponent* pc, Entity::Player player);

	/*Deletes all the Components*/
	void DeleteAllComponents();

	/* Add a component to the entity*/
	Entity* Add(Component * component);

	/* Return the Component associated with the given tag */
	Component* GetComponent(Component::Tag tag);

	/* Remove a component from the entity */
	Component* Remove(Component * component);

	/* Get all assigned Tags of this Entity */
	std::vector<Component::Tag> GetTags();

	/* Get all assigned Components of this Entity */
	std::vector<Component*> GetComponents();

private:
	componentmap components;

};

#endif