#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "System.h"
#include "Engine.h"
#include "AnimationComponent.h"
#include "UnitComponent.h"
#include "Graphics.h"

class AnimationSystem : public System
{
public:
	AnimationSystem() :
		System() {};
	~AnimationSystem() {};

protected:
	virtual void Update();

	virtual Type GetType() { return System::TYPE_ANIMATION; };
};

#endif