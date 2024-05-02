#pragma once
#include "Entity.h"

class Victim;

class Player : public Entity
{
public:
	explicit Player();

	virtual void Draw(const Rectf& viewPort) const;
	virtual void Move(float elapsedSec) override;
	virtual void Action(Victim* victim);

	void Update(float elapsedSec, bool isPlaying);
	void ShowHunger(const Rectf& viewPort) const;

	void  AddHunger(const float hungerIncrease);
	float GetHunger() const;
	int	  GetMaxHunger() const;

	bool IsClose(Victim* victim) const;
	bool HasAttacked() const;

	void SetPosition(const Point2f& newPos);
	Point2f GetPlayerPos() const;
private:
	//enum Classes
	enum class Direction
	{
		Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft
	};

	//Constants
	const int   RADIUS_PLAYER	{  15    };
	const int   MAX_HUNGER		{ 100    };
	const float HUNGER_DECREASE	{  15.f  };
	const int	KILL_RADIUS		{  35    };

	//Variables
	float m_Hunger;
	bool m_HasTarget;

	Point2f m_Position{ 450, 250 };

	Direction m_Direction{ Direction::Right };
};

