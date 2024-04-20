#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	explicit Player();

	virtual void Draw() const override;
	virtual void Move(float elapsedSec) override;
	virtual void Attack() override;
private:
	//enum Classes
	enum class Direction
	{
		Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft
	};

	//Constants
	const int SPEED{ 250 };
	const int RADIUS_PLAYER{ 20 };

	//Variables
	Point2f m_PlayerPos{ 0, 0 };

	Direction m_Direction{ Direction::Right };
};

