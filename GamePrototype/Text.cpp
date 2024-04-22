#include "pch.h"
#include "Text.h"

Text::Text(std::string text, Color4f textColor, TTF_Font* fontPath)
	:m_TextColor(textColor), m_Text(text), m_FontPtr(fontPath)
{
	m_TextTexturePtr = new Texture(m_Text, m_FontPtr, m_TextColor);
}

Text::~Text()
{
	if (m_FontPtr != nullptr)
	{
		TTF_CloseFont(m_FontPtr);
		m_FontPtr = nullptr;
	}

	delete m_TextTexturePtr;
}

void Text::Draw(Point2f position) const
{
	m_TextTexturePtr->Draw(Point2f{ position.x - m_TextTexturePtr->GetWidth() / 2,
									position.y - m_TextTexturePtr->GetHeight() / 2 });
}