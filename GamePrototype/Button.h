#pragma once
#include "utils.h"
#include "Text.h"
using namespace utils;

class Button
{
public:
	Button(Point2f position, std::string text, Color4f textColor, TTF_Font* fontPath);
	~Button();

	void Draw();
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);
	bool IsPressed();
private:

	bool m_Hover{ false };
	bool m_Pressed{ false };

	Rectf	m_MyBounds;
	Point2f m_Position;

	TTF_Font* m_FontPtr;
	Color4f m_TextColor;
	std::string m_Text;
	Texture* m_TextTexturePtr;
};

