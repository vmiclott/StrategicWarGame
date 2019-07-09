#ifndef SCORE_SYSTEM_H
#define SCORE_SYSTEM_H

#include "System.h"
#include "Engine.h"
#include "AllegroLib.h"
#include "UnitComponent.h"
#include "ScoreComponent.h"

class ScoreSystem : public System
{
public:
	ScoreSystem() {};
	~ScoreSystem() {};

protected:
	virtual void Update();

	virtual Type GetType() { return System::TYPE_SCORE; };
	void SetEngine(Engine*);
private:

	//Field with pointer to the score of the current game
	Entity* score;

	//Recalculates the different fields.
	void updateData();

};

#endif
