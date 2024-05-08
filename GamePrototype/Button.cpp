#include "pch.h"
#include "Button.h"

Button::Button(Point2f position, std::string text, Color4f textColor, TTF_Font* fontPath, int btnValue) :
	m_TextColor(textColor), m_Text(text), m_FontPtr(fontPath), m_Position(position), m_Value{btnValue}
{
	m_TextTexturePtr = new Texture(m_Text, m_FontPtr, m_TextColor);

	m_MyBounds.left = position.x;
	m_MyBounds.height = m_TextTexturePtr->GetHeight();
	m_MyBounds.width = m_TextTexturePtr->GetWidth();
	m_MyBounds.bottom = position.y;
}
Button::~Button()
{
	delete m_TextTexturePtr;
}

void Button::Draw()
{
	if (m_Hover == true)
	{
		SetColor(Color4f{ 1.f, 1.f, 1.f, 0.2f });
		FillRect(m_MyBounds);
		m_TextTexturePtr->Draw(Point2f{ m_Position.x, m_Position.y});
	}
	else m_TextTexturePtr->Draw(Point2f{ m_Position.x, m_Position.y});
}
void Button::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	if (IsPointInRect(Point2f{ float(e.x), float(e.y) }, m_MyBounds) == true) m_Hover = true;
	else m_Hover = false;
}

void Button::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	if (IsPointInRect(Point2f{ float(e.x), float(e.y) }, m_MyBounds) == true) m_Pressed = true;
}
void Button::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	if (IsPointInRect(Point2f{ float(e.x), float(e.y) }, m_MyBounds) == true) m_Pressed = false;
}

bool Button::IsPressed()
{
	return m_Pressed;
}

int Button::GetValue()
{
	return m_Value;
}

