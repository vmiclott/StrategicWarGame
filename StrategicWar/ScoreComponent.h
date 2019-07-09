#ifndef SCORE_COMPONENT_H
#define SCORE_COMPONENT_H

#include "Component.h"
#include "Graphics.h"

class ScoreComponent : public Component
{
public:
	
	int playerUnits;
	int aiUnits;
	int totalPlayerHqHealth;
	int totalAiHqHealth;
	bool playerWon;
	bool aiWon;

	ScoreComponent(int _playerUnits, int _aiUnits, int _totalPlayerHqHealth, int _totalAiHqHealth) :
		playerUnits(_playerUnits), aiUnits(_aiUnits), totalPlayerHqHealth(_totalPlayerHqHealth), totalAiHqHealth(_totalAiHqHealth)
	{
		playerWon = false;
		aiWon = false;
	};

	virtual ~ScoreComponent() {};
	virtual Tag GetTag() { return SCORE; };
};

#endif