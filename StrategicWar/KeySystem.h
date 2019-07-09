#ifndef KEY_SYSTEM_H
#define KEY_SYSTEM_H

#include "System.h"
#include "Engine.h"
#include "AllegroLib.h"
#include "Context.h"

class KeySystem : public System
{
public:
	KeySystem();
	~KeySystem() {};

protected:
	virtual void Update();

	virtual Type GetType() { return System::TYPE_KEY; };

private:

	//Field with allegroLib to handle the mouse events.
	AllegroLib* al;

};

#endif
