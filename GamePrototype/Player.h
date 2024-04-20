#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	explicit Player();

	virtual void Draw() const override;
	virtual void Move(float elapsedSec) override;
	virtual void Action() override;

	void Update(float elapsedSec);
	void ShowHunger() const;

	void  AddHunger(const float hungerIncrease);
	float GetHunger() const;
private:
	//enum Classes
	enum class Direction
	{
		Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft
	};

	//Constants
	const int   RADIUS_PLAYER	{  15    };
	const int   MAX_HUNGER		{ 100    };
	const float HUNGER_DECREASE	{   5.f };

	//Variables
	float m_Hunger;
	Point2f m_PlayerPos{ 10, 10 };
	

	Direction m_Direction{ Direction::Right };
};

