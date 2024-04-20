#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	explicit Player();

	virtual void Draw() const override;
	virtual void Move(float elapsedSec) override;
	virtual void Action() override;
private:
	//enum Classes
	enum class Direction
	{
		Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft
	};

	//Constants
	const int SPEED			{ 250 };
	const int RADIUS_PLAYER	{  20 };
	const int MAX_HUNGER	{ 100 };

	//Variables
	int m_Hunger;
	Point2f m_PlayerPos{ 10, 10 };
	

	Direction m_Direction{ Direction::Right };
};

