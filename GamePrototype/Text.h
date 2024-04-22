#pragma once
#include "Texture.h"
class Text
{
public:
	Text(std::string text, Color4f textColor, TTF_Font* fontPath);
	~Text();

	virtual void Draw(Point2f position) const;

protected:
	TTF_Font* m_FontPtr;
	Color4f m_TextColor;
	std::string m_Text;
	Texture* m_TextTexturePtr;
};

