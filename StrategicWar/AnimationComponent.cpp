#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(std::vector<Grid> path, Vector2 startPos)
{
	this->path = path;
	this->startPos = startPos;
	progress = 0;
}

AnimationComponent::AnimationComponent(Vector2 startPos, Vector2 endPos)
{
	this->startPos = startPos;
	this->endPos = endPos;
}

AnimationComponent::~AnimationComponent()
{
}

Component::Tag AnimationComponent::GetTag()
{
	return ANIMATION;
}


