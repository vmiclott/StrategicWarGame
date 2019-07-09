#ifndef HOVER_COMPONENT_H
#define HOVER_COMPONENT_H

#include "Component.h"

class PauseComponent : public Component
{
public:

    PauseComponent() {};
    virtual ~PauseComponent() {};
    virtual Tag GetTag() { return PAUSE; };


};
#endif
#pragma once
