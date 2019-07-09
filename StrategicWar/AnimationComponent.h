#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "Component.h"
#include "Graphics.h"

class AnimationComponent : public Component
{
public:
	std::vector<Grid> path;
	Vector2 startPos;
	Vector2 endPos;
	int progress;

	AnimationComponent(std::vector<Grid> path, Vector2 startPos);
	AnimationComponent(Vector2 startPos, Vector2 endPos);

	virtual ~AnimationComponent();
	virtual Tag GetTag(); 
};

#endif
