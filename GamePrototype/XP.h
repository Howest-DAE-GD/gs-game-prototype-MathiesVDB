#pragma once

enum class Type
{
	Small, Medium, Large
};

class XP final
{
public:
	XP(Point2f pos, Type xpType);

	void Draw() const;

	int GetValue() const;
	Circlef GetXPHitbox();

	//Constants
	static const int RADIUS{ 6 };
private:
	Type m_XPType;

	int m_Value;

	Point2f m_Position;
};

