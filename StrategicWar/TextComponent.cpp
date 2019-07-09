#include "TextComponent.h"

using namespace std;
TextComponent::TextComponent(string text)
{
	this->text = text;
}

TextComponent::~TextComponent() {}

Component::Tag TextComponent::GetTag()
{
	return Component::TEXT;
}

