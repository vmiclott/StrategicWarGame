#ifndef SELECTION_COMPONENT_H
#define SELECTION_COMPONENT_H

#include "Component.h"

class SelectionComponent : public Component
{
public:

    enum SelectionComponentState {
        NEW,
        OLD, 
        TO_DELETE
    };

    SelectionComponentState state;

    SelectionComponent() : state(SelectionComponent::NEW) {};
    virtual ~SelectionComponent() {};
    virtual Tag GetTag() { return SELECTION; };


};
#endif
