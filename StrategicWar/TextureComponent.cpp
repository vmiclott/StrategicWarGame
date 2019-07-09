#include "TextureComponent.h"

using namespace std;
TextureComponent::TextureComponent(vector<Graphics::Sprite> sprites)
{
	for (Graphics::Sprite sprite : sprites)
	{
		textures.push_back(sprite);
	}
}

TextureComponent::TextureComponent(Color color)
{
	this->color = color;
	textures.push_back(Graphics::SPRITE_NONE);
}

TextureComponent::~TextureComponent() {}

Component::Tag TextureComponent::GetTag()
{
	return Component::TEXTURE;
}

void TextureComponent::addTexture(Graphics::Sprite sprite)
{
	textures.push_back(sprite);
}

void TextureComponent::removeTexture(Graphics::Sprite sprite)
{
	vector<Graphics::Sprite>::iterator it = textures.begin();
	while (it != textures.end()) {
		if (*it == sprite) {
			textures.erase(it);
			return;
		}
		it++;
	}
	
}
