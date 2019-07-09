#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include "Component.h"
#include "Graphics.h"
#include "Color.h"

class TextureComponent : public Component
{
public:
	std::vector<Graphics::Sprite> textures;
	Color color;

	TextureComponent(std::vector<Graphics::Sprite> sprites);
	TextureComponent(Color color);
	TextureComponent() {};
	virtual ~TextureComponent();
	virtual Tag GetTag();
	void addTexture(Graphics::Sprite sprite);
	void removeTexture(Graphics::Sprite sprite);
};

#endif