#pragma once
#include "utils.h"
#include "Text.h"
using namespace utils;

class Button
{
public:
	Button(Point2f position, std::string text, Color4f textColor, TTF_Font* fontPath, int btnValue = -1);
	~Button();

	void Draw();
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);
	void ReleaseButton();
	bool IsPressed();
	int  GetValue();
private:

	int  m_Value;
	bool m_Hover{ false };
	bool m_Pressed{ false };

	Rectf	m_MyBounds;
	Point2f m_Position;

	TTF_Font* m_FontPtr;
	Color4f m_TextColor;
	std::string m_Text;
	Texture* m_TextTexturePtr;
};

