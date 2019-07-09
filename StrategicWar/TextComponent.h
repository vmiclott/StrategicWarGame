#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "Component.h"
#include "Graphics.h"
#include "Color.h"

class TextComponent : public Component
{
public:
	std::string text;

	TextComponent(std::string text);
	virtual ~TextComponent();
	virtual Tag GetTag();
};

#endif
