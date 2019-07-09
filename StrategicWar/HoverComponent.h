#ifndef HOVER_COMPONENT_H
#define HOVER_COMPONENT_H

#include "Component.h"

class HoverComponent : public Component
{
public:

    HoverComponent() {};
    virtual ~HoverComponent() {};
    virtual Tag GetTag() { return HOVER; };


};
#endif
